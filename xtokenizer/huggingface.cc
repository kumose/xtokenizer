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

#include <xtokenizer/huggingface.h>
#ifdef RUST_SUPPORT

namespace xtokenizer {

    HuggingTokenizer::~HuggingTokenizer()  {
        if (handle_ != nullptr) {
            tokenizers_free(handle_);
            handle_ = nullptr;
        }
    }

    ///
    /// @brief Encode text into ids.
    /// @param text The input text.
    /// @returns The encoded token ids.
    ///
    std::vector<int32_t> HuggingTokenizer::encode(std::string_view text,  bool add_special_tokens) {
        TokenizerEncodeResult result;
        tokenizers_encode(handle_, text.data(), text.length(), static_cast<int>(add_special_tokens),
                          &result);
        std::vector<int32_t> ret(result.token_ids, result.token_ids + result.len);
        tokenizers_free_encode_results(&result, 1);
        return ret;
    }

    std::vector<std::vector<int32_t>> HuggingTokenizer::encode_batch(const std::vector<std::string>& texts,
                                              bool add_special_tokens) {
        std::vector<const char*> texts_raw;
        std::vector<size_t> seq_lens;
        size_t num_seqs = texts.size();
        texts_raw.reserve(num_seqs);
        seq_lens.reserve(num_seqs);
        for (const auto& text : texts) {
            texts_raw.push_back(text.data());
            seq_lens.push_back(text.length());
        }
        std::vector<TokenizerEncodeResult> results(num_seqs);
        tokenizers_encode_batch(handle_, texts_raw.data(), seq_lens.data(), texts.size(),
                                static_cast<int>(add_special_tokens), results.data());
        std::vector<std::vector<int32_t>> ret;
        ret.reserve(texts.size());
        for (size_t i = 0; i < texts.size(); ++i) {
            ret.push_back(
                std::vector<int32_t>(results[i].token_ids, results[i].token_ids + results[i].len));
        }
        tokenizers_free_encode_results(results.data(), texts.size());
        return ret;
    }

    ///
    /// @brief Decode token ids into text.
    /// @param ids The token ids.
    /// @returns The decoded text.
    ///
    std::string HuggingTokenizer::decode(const std::vector<int32_t>& ids, bool skip_special_tokens) {
        tokenizers_decode(handle_, reinterpret_cast<const uint32_t*>(ids.data()), ids.size(),
                          static_cast<int>(skip_special_tokens));
        const char* data;
        size_t len;
        tokenizers_get_decode_str(handle_, &data, &len);
        return std::string(data, len);
    }

    ///
    /// brief Returns the vocabulary size. Special tokens are considered.
    ///
    size_t HuggingTokenizer::get_vocab_size() {
        size_t size;
        tokenizers_get_vocab_size(handle_, &size);
        assert(size > 0);
        return size;
    }

    ///
    /// @brief Convert the given id to its corresponding token if it exists. If not, return an
    ///  empty string.
    ///
    std::string HuggingTokenizer::id_to_token(int32_t token_id) {
        const char* data;
        size_t len;
        tokenizers_id_to_token(handle_, static_cast<uint32_t>(token_id), &data, &len);
        return std::string(data, len);
    }


    ///
    /// @brief Convert the given token to its corresponding id if it exists. If not, return -1.
    ///
    int32_t HuggingTokenizer::token_to_id(const std::string &token) {
        int32_t id;
        tokenizers_token_to_id(handle_, token.data(), token.length(), &id);
        return id;
    }

    ///
    /// @brief the name of the tokenizer.
    std::string HuggingTokenizer::name() {
        return "sentencepiece";
    }

    ///
    /// @brief pass a variable config string to user,
    ///        weather or how to, all by user.
    turbo::Status HuggingTokenizer::initialize(std::string_view dir_or_path_or_json) {

        handle_ = tokenizers_new_from_str(dir_or_path_or_json.data(), dir_or_path_or_json.length());
        if (!handle_) {
            return turbo::invalid_argument_error("check your data for huggingface tokenizer");
        }
#ifdef COMPILE_WASM_RUNTIME
        setenv("TOKENIZERS_PARALLELISM", "false", true);
#endif
        return turbo::OkStatus();;
    }

    turbo::Status HuggingTokenizer::initialize(const std::string& vocab,
                                                           const std::string& merges,
                                                           const std::string& added_tokens) {
        handle_ = byte_level_bpe_tokenizers_new_from_str(
            vocab.data(), vocab.length(), merges.data(), merges.length(), added_tokens.data(),
            added_tokens.length());
        if (!handle_) {
            return turbo::invalid_argument_error("check your data for huggingface in BPE module tokenizer");
        }
#ifdef COMPILE_WASM_RUNTIME
        setenv("TOKENIZERS_PARALLELISM", "false", true);
#endif
        return turbo::OkStatus();;
    }
}  // namespace  xtokenizer
#endif  // RUST_SUPPORT
