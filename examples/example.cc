///
/// Copyright (c) 2023 by Contributors
/// file tokenizers_c.h
/// brief C binding to tokenizers rust library
///
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

#include <xtokenizer/tokenizer.h>

#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using xtokenizer::Tokenizer;

std::string LoadBytesFromFile(const std::string &path) {
    std::ifstream fs(path, std::ios::in | std::ios::binary);
    if (fs.fail()) {
        std::cerr << "Cannot open " << path << std::endl;
        exit(1);
    }
    std::string data;
    fs.seekg(0, std::ios::end);
    size_t size = static_cast<size_t>(fs.tellg());
    fs.seekg(0, std::ios::beg);
    data.resize(size);
    fs.read(data.data(), size);
    return data;
}

void PrintEncodeResult(const std::vector<int> &ids) {
    std::cout << "tokens=[";
    for (size_t i = 0; i < ids.size(); ++i) {
        if (i != 0) std::cout << ", ";
        std::cout << ids[i];
    }
    std::cout << "]" << std::endl;
}

void TestTokenizer(std::unique_ptr<Tokenizer> tok, bool print_vocab = false,
                   bool check_id_back = true) {
    // Check #1. Encode and Decode
    std::string prompt = "What is the  capital of Canada?";
    std::vector<int> ids = tok->encode(prompt);
    std::string decoded_prompt = tok->decode(ids);
    PrintEncodeResult(ids);
    std::cout << "decode=\"" << decoded_prompt << "\"" << std::endl;
    assert(decoded_prompt == prompt);

    // Check #2. IdToToken and TokenToId
    std::vector<int32_t> ids_to_test = {0, 1, 2, 3, 32, 33, 34, 130, 131, 1000};
    for (auto id: ids_to_test) {
        auto token = tok->id_to_token(id);
        auto id_new = tok->token_to_id(token);
        std::cout << "id=" << id << ", token=\"" << token << "\", id_new=" << id_new << std::endl;
        if (check_id_back) {
            assert(id == id_new);
        }
    }

    // Check #3. GetVocabSize
    auto vocab_size = tok->get_vocab_size();
    std::cout << "vocab_size=" << vocab_size << std::endl;

    std::cout << std::endl;
}

// Sentencepiece tokenizer
// - dist/tokenizer.model
void SentencePieceTokenizerExample() {
    std::cout << "Tokenizer: SentencePiece" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // Read blob from file.
    auto blob = LoadBytesFromFile("dist/tokenizer.model");
    // Note: all the current factory APIs takes in-memory blob as input.
    // This gives some flexibility on how these blobs can be read.
    auto tok = xtokenizer::Tokenizer::sentence_piece_from_blob(blob);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Load time: " << duration << " ms" << std::endl;

    TestTokenizer(std::move(tok), false, true);
}

// HF tokenizer
// - dist/tokenizer.json
void HuggingFaceTokenizerExample() {
    std::cout << "Tokenizer: Huggingface" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // Read blob from file.
    auto blob = LoadBytesFromFile("dist/tokenizer.json");
    // Note: all the current factory APIs takes in-memory blob as input.
    // This gives some flexibility on how these blobs can be read.
    auto tok = xtokenizer::Tokenizer::hugging_from_blob_json(blob);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Load time: " << duration << " ms" << std::endl;

    TestTokenizer(std::move(tok), false, true);
}

void HuggingFaceBPETokenizerExample() {
    std::cout << "Tokenizer: Huggingface BPE" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // Read blob from file.
    auto vocab_blob = LoadBytesFromFile("dist/vocab.json");
    auto merges_blob = LoadBytesFromFile("dist/merges.txt");
    // Note: all the current factory APIs takes in-memory blob as input.
    // This gives some flexibility on how these blobs can be read.
    auto tok = xtokenizer::Tokenizer::hugging_from_blob_byte_level_bpe(vocab_blob, merges_blob);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Load time: " << duration << " ms" << std::endl;

    TestTokenizer(std::move(tok), false, true);
}

int main(int argc, char *argv[]) {
    SentencePieceTokenizerExample();
    HuggingFaceTokenizerExample();
    HuggingFaceBPETokenizerExample();
}
