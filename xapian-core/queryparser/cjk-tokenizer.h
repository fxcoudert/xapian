/** @file cjk-tokenizer.h
 * @brief Tokenise CJK text as n-grams
 */
/* Copyright (c) 2007, 2008 Yung-chung Lin (henearkrxern@gmail.com)
 * Copyright (c) 2011 Richard Boulton (richard@tartarus.org)
 * Copyright (c) 2011 Brandon Schaefer (brandontschaefer@gmail.com)
 * Copyright (c) 2011,2018,2019 Olly Betts
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef XAPIAN_INCLUDED_CJK_TOKENIZER_H
#define XAPIAN_INCLUDED_CJK_TOKENIZER_H

#include "xapian/unicode.h"

#include <string>

namespace CJK {

/** Should we use the CJK n-gram code?
 *
 *  The first time this is called it reads the environment variable
 *  XAPIAN_CJK_NGRAM and returns true if it is set to a non-empty value.
 *  Subsequent calls cache and return the same value.
 */
bool is_cjk_enabled();

bool codepoint_is_cjk(unsigned codepoint);

size_t get_cjk(Xapian::Utf8Iterator& it);

}

/// Iterator returning unigrams and bigrams.
class CJKTokenIterator {
    Xapian::Utf8Iterator it;

    /** Offset to penultimate Unicode character in current_token.
     *
     *  If current_token has one Unicode character, this is 0.
     */
    unsigned offset = 0;

    std::string current_token;

    /// Call to set current_token at the start.
    void init();

  public:
    explicit CJKTokenIterator(const std::string& s) : it(s) {
	init();
    }

    explicit CJKTokenIterator(const Xapian::Utf8Iterator& it_) : it(it_) {
	init();
    }

    CJKTokenIterator() { }

    const std::string& operator*() const {
	return current_token;
    }

    CJKTokenIterator& operator++();

    /// Is this a unigram?
    bool unigram() const { return offset == 0; }

    const Xapian::Utf8Iterator& get_utf8iterator() const { return it; }

    bool operator==(const CJKTokenIterator& other) const {
	// We only really care about comparisons where one or other is an end
	// iterator.
	return current_token.empty() && other.current_token.empty();
    }

    bool operator!=(const CJKTokenIterator& other) const {
	return !(*this == other);
    }
};

#endif // XAPIAN_INCLUDED_CJK_TOKENIZER_H
