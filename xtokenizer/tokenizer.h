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

#include <string>
#include <vector>
#include <string_view>
#include <turbo/container/span.h>
#include <turbo/utility/status.h>

namespace xtokenizer {

    ////////////////////////////////////////////////////
    ///  Tokenizer
    /// brief a universal tokenizer that loads
    /// either HF's tokenizer or sentence piece, depending on the constructor.
    class Tokenizer {
    public:
        /// virtual destructor
        virtual ~Tokenizer() = default;

        ///
        /// @brief Encode text into ids.
        /// @param text The input text.
        /// @returns The encoded token ids.
        ///
        virtual std::vector<int32_t> encode(std::string_view text) = 0;

        ///
        /// @brief Encode a batch of texts into ids.
        /// @param texts The input texts.
        /// @returns The encoded token ids.
        ///
        virtual std::vector<std::vector<int32_t>> encode_batch(const std::vector<std::string> &texts) {
            // Fall back when the derived class does not implement this function.
            std::vector<std::vector<int32_t>> ret;
            ret.reserve(texts.size());
            for (const auto& text : texts) {
                ret.push_back(encode(text));
            }
            return ret;
        }

        virtual std::vector<std::vector<int32_t>> encode_batch(turbo::span<std::string> texts) {
            // Fall back when the derived class does not implement this function.
            std::vector<std::vector<int32_t>> ret;
            ret.reserve(texts.size());
            for (const auto& text : texts) {
                ret.push_back(encode(text));
            }
            return ret;
        }

        virtual std::vector<std::vector<int32_t>> encode_batch(const std::vector<std::string_view> &texts) {
            // Fall back when the derived class does not implement this function.
            std::vector<std::vector<int32_t>> ret;
            ret.reserve(texts.size());
            for (const auto& text : texts) {
                ret.push_back(encode(text));
            }
            return ret;
        }

        virtual std::vector<std::vector<int32_t>> encode_batch(turbo::span<std::string_view> texts) {
            // Fall back when the derived class does not implement this function.
            std::vector<std::vector<int32_t>> ret;
            ret.reserve(texts.size());
            for (const auto& text : texts) {
                ret.push_back(encode(text));
            }
            return ret;
        }

        ///
        /// @brief Decode token ids into text.
        /// @param text The token ids.
        /// @returns The decoded text.
        ///
        virtual std::string decode(const std::vector<int32_t> & ids) = 0;


        ///
        /// brief Returns the vocabulary size. Special tokens are considered.
        ///
        virtual size_t get_vocab_size() = 0;

        ///
        /// @brief Convert the given id to its corresponding token if it exists. If not, return an
        ///  empty string.
        ///
        virtual std::string id_to_token(int32_t token_id) = 0;


        ///
        /// @brief Convert the given token to its corresponding id if it exists. If not, return -1.
        ///
        virtual int32_t token_to_id(const std::string& token) = 0;

        ///
        /// @brief the name of the tokenizer.
        virtual std::string name() = 0;
        ///
        /// @brief pass a variable config string to user,
        ///        weather or how to, all by user.
        virtual turbo::Status initialize(std::string_view dir_or_path_or_json) = 0;

        //////////////////////////////////////////////////////////////////////////////////////////////////////
        /// brief Create SentencePiece.
        ///
        /// @param model_blob The blob that contains vocabs.
        /// @return The created tokenizer.
        ///
        static std::unique_ptr<Tokenizer> sentence_piece_from_blob(const std::string& model_blob);

        static std::unique_ptr<Tokenizer> hugging_from_blob_json(const std::string& json);

        static std::unique_ptr<Tokenizer> hugging_from_blob_byte_level_bpe(const std::string& vocab,
                                                                const std::string& merges,
                                                                const std::string& added_tokens = "");
    };

} // namespace xtokenizer
