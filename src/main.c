#include <ctype.h>
#include <curl/curl.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LANGS_COUNT 5
#define MIN_REQUIRED_ARGS 2
#define MAX_ALLOWED_ARGS 4
#define CLOSE_TEXT_TAG "</text>"

typedef struct {
    char *data;
    size_t len;
} mem_buf_t;

typedef struct {
    char **items;
    size_t len;
    size_t cap;
} str_vec_t;

static size_t write_callback(char *contents, size_t size, size_t nmemb, void *user_data) {
    size_t chunk_size = size * nmemb;
    mem_buf_t *buffer = (mem_buf_t *)user_data;

    char *new_data = realloc(buffer->data, buffer->len + chunk_size + 1);
    if (new_data == NULL) {
        return 0;
    }

    buffer->data = new_data;
    memcpy(buffer->data + buffer->len, contents, chunk_size);
    buffer->len += chunk_size;
    buffer->data[buffer->len] = '\0';

    return chunk_size;
}

static bool vec_push(str_vec_t *vec, char *value) {
    if (vec->len == vec->cap) {
        size_t new_cap = (vec->cap == 0) ? 8 : vec->cap * 2;
        char **new_items = realloc(vec->items, new_cap * sizeof(*new_items));
        if (new_items == NULL) {
            return false;
        }

        vec->items = new_items;
        vec->cap = new_cap;
    }

    vec->items[vec->len++] = value;
    return true;
}

static void vec_free(str_vec_t *vec) {
    for (size_t i = 0; i < vec->len; ++i) {
        free(vec->items[i]);
    }

    free(vec->items);
    vec->items = NULL;
    vec->len = 0;
    vec->cap = 0;
}

static char *trimmed_copy(const char *begin, const char *end) {
    while (begin < end && isspace((unsigned char)*begin)) {
        ++begin;
    }

    while (end > begin && isspace((unsigned char)*(end - 1))) {
        --end;
    }

    size_t len = (size_t)(end - begin);
    char *out = malloc(len + 1);
    if (out == NULL) {
        return NULL;
    }

    if (len > 0) {
        memcpy(out, begin, len);
    }
    out[len] = '\0';

    return out;
}

static bool extract_text_values(const char *body, str_vec_t *values) {
    const char *cursor = body;

    while ((cursor = strstr(cursor, "<text")) != NULL) {
        const char *line_end = strchr(cursor, '\n');
        const char *open_end = strchr(cursor, '>');
        if (open_end == NULL) {
            break;
        }

        // Match Rust regex behavior: skip <text ...> tags that span multiple lines.
        if (line_end != NULL && line_end < open_end) {
            cursor = line_end + 1;
            continue;
        }

        const char *close = strstr(open_end + 1, CLOSE_TEXT_TAG);
        if (close == NULL) {
            break;
        }

        if (line_end != NULL && line_end < close) {
            cursor = line_end + 1;
            continue;
        }

        char *value = trimmed_copy(open_end + 1, close);
        if (value == NULL) {
            return false;
        }

        if (!vec_push(values, value)) {
            free(value);
            return false;
        }

        cursor = close + strlen(CLOSE_TEXT_TAG);
    }

    return true;
}

static int parse_langs_count(const char *input) {
    char *end = NULL;
    errno = 0;
    long value = strtol(input, &end, 10);

    if (errno != 0 || end == input || *end != '\0' || value < INT_MIN || value > INT_MAX) {
        return DEFAULT_LANGS_COUNT;
    }

    return (int)value;
}

static size_t digit_count(size_t value) {
    size_t digits = 1;

    while (value >= 10) {
        value /= 10;
        ++digits;
    }

    return digits;
}

static char *build_url(const char *username, int langs_count, const char *hide) {
    const char *pattern = "https://github-readme-stats.vercel.app/api/top-langs/?username=%s&langs_count=%d&hide=%s";
    int needed = snprintf(NULL, 0, pattern, username, langs_count, hide);
    if (needed < 0) {
        return NULL;
    }

    char *url = malloc((size_t)needed + 1);
    if (url == NULL) {
        return NULL;
    }

    snprintf(url, (size_t)needed + 1, pattern, username, langs_count, hide);
    return url;
}

static bool fetch_url(const char *url, char **out_body) {
    CURL *curl = curl_easy_init();
    if (curl == NULL) {
        fprintf(stderr, "Unable to initialize libcurl.\n");
        return false;
    }

    mem_buf_t buffer = {
        .data = calloc(1, 1),
        .len = 0,
    };

    if (buffer.data == NULL) {
        curl_easy_cleanup(curl);
        fprintf(stderr, "Unable to allocate response buffer.\n");
        return false;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "github-readme-stats-cli-c/1.0");

    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(res));
        free(buffer.data);
        return false;
    }

    *out_body = buffer.data;
    return true;
}

int main(int argc, char **argv) {
    if (argc < MIN_REQUIRED_ARGS || argc > MAX_ALLOWED_ARGS) {
        fprintf(stderr, "Usage: %s <username> <optional=[langs_count], [hide]>\n", argv[0]);
        return 1;
    }

    const char *username = argv[1];
    int langs_count = (argc >= 3) ? parse_langs_count(argv[2]) : DEFAULT_LANGS_COUNT;
    const char *hide = (argc >= 4) ? argv[3] : "";

    char *url = build_url(username, langs_count, hide);
    if (url == NULL) {
        fprintf(stderr, "Unable to allocate URL buffer.\n");
        return 1;
    }

    CURLcode init_code = curl_global_init(CURL_GLOBAL_DEFAULT);
    if (init_code != CURLE_OK) {
        fprintf(stderr, "libcurl global init failed: %s\n", curl_easy_strerror(init_code));
        free(url);
        return 1;
    }

    char *body = NULL;
    if (!fetch_url(url, &body)) {
        curl_global_cleanup();
        free(url);
        return 1;
    }

    str_vec_t values = {0};
    if (!extract_text_values(body, &values)) {
        fprintf(stderr, "Unable to parse SVG response due to memory allocation failure.\n");
        free(body);
        free(url);
        curl_global_cleanup();
        vec_free(&values);
        return 1;
    }

    size_t rows = values.len / 2;
    size_t rank_width = digit_count(rows);
    if (rank_width < 2) {
        rank_width = 2;
    }

    size_t name_width = 11;
    size_t pct_width = 7;

    // The API returns alternating <text> values: language name, then percentage.
    for (size_t i = 0; i + 1 < values.len; i += 2) {
        size_t name_len = strlen(values.items[i]);
        size_t pct_len = strlen(values.items[i + 1]);

        if (name_len > name_width) {
            name_width = name_len;
        }
        if (pct_len > pct_width) {
            pct_width = pct_len;
        }
    }

    printf("%s Most Used Languages\n", username);
    for (size_t i = 0; i < rows; ++i) {
        const char *name = values.items[i * 2];
        const char *pct = values.items[i * 2 + 1];

        printf(
            "%*zu - %-*s [ %*s ]\n",
            (int)rank_width,
            i + 1,
            (int)name_width,
            name,
            (int)pct_width,
            pct
        );
    }

    vec_free(&values);
    free(body);
    free(url);
    curl_global_cleanup();

    return 0;
}
