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

#include <xtokenizer/sentencepiece.h>


namespace xtokenizer {

    ///
    /// @brief Encode text into ids.
    /// @param text The input text.
    /// @returns The encoded token ids.
    ///
    std::vector<int32_t> SentencePieceTokenizer::encode(std::string_view text) {
        std::vector<int32_t> tokens;
        sentence_piece_.Encode(text, &tokens).IgnoreError();
        return tokens;
    }

    ///
    /// @brief Decode token ids into text.
    /// @param ids The token ids.
    /// @returns The decoded text.
    ///
    std::string SentencePieceTokenizer::decode(const std::vector<int32_t> &ids) {
        std::string text;
        sentence_piece_.Decode(ids, &text).IgnoreError();
        return text;
    }

    ///
    /// brief Returns the vocabulary size. Special tokens are considered.
    ///
    size_t SentencePieceTokenizer::get_vocab_size() {
        auto size = sentence_piece_.GetPieceSize();
        assert(size > 0);
        return size;
    }

    ///
    /// @brief Convert the given id to its corresponding token if it exists. If not, return an
    ///  empty string.
    ///
    std::string SentencePieceTokenizer::id_to_token(int32_t token_id) {
        return sentence_piece_.IdToPiece(token_id);
    }


    ///
    /// @brief Convert the given token to its corresponding id if it exists. If not, return -1.
    ///
    int32_t SentencePieceTokenizer::token_to_id(const std::string &token) {
        return sentence_piece_.PieceToId(token);
    }

    ///
    /// @brief the name of the tokenizer.
    std::string SentencePieceTokenizer::name() {
        return "sentencepiece";
    }

    ///
    /// @brief pass a variable config string to user,
    ///        weather or how to, all by user.
    turbo::Status SentencePieceTokenizer::initialize(std::string_view dir_or_path_or_json) {
        auto rs = sentence_piece_.LoadFromSerializedProto(dir_or_path_or_json);
        if (!rs.ok()) {
            return turbo::make_status(rs.code(), rs.message());
        }
        return turbo::OkStatus();;
    }


}  // namespace xtokenizer
