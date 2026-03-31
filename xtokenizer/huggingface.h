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

#include <xtokenizer/hg_tokenizers.h>
#ifdef RUST_SUPPORT
#include <xtokenizer/tokenizer.h>

namespace xtokenizer {
    class HuggingTokenizer : public Tokenizer {
    public:
        HuggingTokenizer() = default;

        ~HuggingTokenizer() override;

        ///
        /// @brief Encode text into ids.
        /// @param text The input text.
        /// @returns The encoded token ids.
        ///
        std::vector<int32_t> encode(std::string_view text) override {
            return encode(text, false);
        }

        std::vector<int32_t> encode(std::string_view text, bool add_special_tokens);

        std::vector<std::vector<int32_t> > encode_batch(const std::vector<std::string> &texts,
                                                        bool add_special_tokens);

        std::vector<std::vector<int32_t> > encode_batch(const std::vector<std::string> &texts) override {
            return encode_batch(texts, false);
        }

        ///
        /// @brief Decode token ids into text.
        /// @param ids The token ids.
        /// @returns The decoded text.
        ///
        std::string decode(const std::vector<int32_t> &ids) override {
            return decode(ids, false);
        }

        // use i32 to be consistent with sentencepiece
        std::string decode(const std::vector<int32_t> &ids, bool skip_special_tokens);

        ///
        /// brief Returns the vocabulary size. Special tokens are considered.
        ///
        size_t get_vocab_size() override;

        ///
        /// @brief Convert the given id to its corresponding token if it exists. If not, return an
        ///  empty string.
        ///
        std::string id_to_token(int32_t token_id) override;


        ///
        /// @brief Convert the given token to its corresponding id if it exists. If not, return -1.
        ///
        int32_t token_to_id(const std::string &token) override;

        ///
        /// @brief the name of the tokenizer.
        std::string name() override;

        ///
        /// @brief path should be dict path
        turbo::Status initialize(std::string_view path) override;

        /// from blob byte level BPE
        turbo::Status initialize(const std::string &vocab, const std::string &merges, const std::string &added_tokens);

    private:
        // the tokenizer
        TokenizerHandle handle_{nullptr};
    };
} // namespace  xtokenizer
#endif  // RUST_SUPPORT
