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
#include <xtokenizer/huggingface.h>


namespace xtokenizer {

    std::unique_ptr<Tokenizer> Tokenizer::sentence_piece_from_blob(const std::string& model_blob) {
        auto ptr = std::make_unique<SentencePieceTokenizer>();
        auto rs = ptr->initialize(model_blob);
        if (!rs.ok()) {
            return nullptr;
        }
        return ptr;
    }
    std::unique_ptr<Tokenizer> Tokenizer::hugging_from_blob_json(const std::string& json) {
        auto ptr = std::make_unique<HuggingTokenizer>();
        auto rs = ptr->initialize(json);
        if (!rs.ok()) {
            return nullptr;
        }
        return ptr;
    }

    std::unique_ptr<Tokenizer> Tokenizer::hugging_from_blob_byte_level_bpe(const std::string& vocab,
                                                       const std::string& merges,
                                                       const std::string& added_tokens) {
        auto ptr = std::make_unique<HuggingTokenizer>();
        auto rs = ptr->initialize(vocab, merges, added_tokens);
        if (!rs.ok()) {
            return nullptr;
        }
        return ptr;
    }
}  // namespace xtokenizer
