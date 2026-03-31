///
/// Copyright (c) 2023 by Contributors
/// file tokenizers_c.h
/// brief C binding to tokenizers rust library
///
// Copyright (C) 2026 Kumo inc. and its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#include <xtokenizer/version.h>

#ifdef RUST_SUPPORT
// The C API
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

typedef void* TokenizerHandle;

typedef struct {
    int* token_ids;
    size_t len;
} TokenizerEncodeResult;

TokenizerHandle tokenizers_new_from_str(const char* json, size_t len);

TokenizerHandle byte_level_bpe_tokenizers_new_from_str(const char* vocab, size_t vocab_len,
                                                       const char* merges, size_t merges_len,
                                                       const char* added_tokens,
                                                       size_t added_tokens_len);

void tokenizers_encode(TokenizerHandle handle, const char* data, size_t len, int add_special_token, TokenizerEncodeResult* result);

void tokenizers_encode_batch(TokenizerHandle handle, const char** data, size_t* len, size_t num_seqs,
                                 int add_special_token, TokenizerEncodeResult* results);

void tokenizers_free_encode_results(TokenizerEncodeResult* results, size_t num_seqs);

void tokenizers_decode(TokenizerHandle handle, const uint32_t* data, size_t len,
                       int skip_special_token);

void tokenizers_get_decode_str(TokenizerHandle handle, const char** data, size_t* len);

void tokenizers_get_vocab_size(TokenizerHandle handle, size_t* size);

void tokenizers_id_to_token(TokenizerHandle handle, uint32_t id, const char** data, size_t* len);

// tokenizers_token_to_id stores -1 to *id if the token is not in the vocab
void tokenizers_token_to_id(TokenizerHandle handle, const char* token, size_t len, int32_t* id);

void tokenizers_free(TokenizerHandle handle);

#ifdef __cplusplus
}
#endif
#endif // RUST_SUPPORT
