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

#include <xtokenizer/tokenizer.h>
#include <sentencepiece_processor.h>

namespace xtokenizer {
    class SentencePieceTokenizer : public Tokenizer {
    public:
        SentencePieceTokenizer() = default;

        ~SentencePieceTokenizer() override = default;

        ///
        /// @brief Encode text into ids.
        /// @param text The input text.
        /// @returns The encoded token ids.
        ///
        std::vector<int32_t> encode(std::string_view text) override;

        ///
        /// @brief Decode token ids into text.
        /// @param ids The token ids.
        /// @returns The decoded text.
        ///
        std::string decode(const std::vector<int32_t> &ids) override;

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

    private:
        // the tokenizer
        sentencepiece::SentencePieceProcessor sentence_piece_;
    };
} // namespace xtokenizer
