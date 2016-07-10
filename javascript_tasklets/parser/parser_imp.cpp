// automatically generated from javascript_tasklets/parser/parser_imp.peg
#line 1 "javascript_tasklets/parser/parser_imp.peg"
              
/*
 * Copyright (C) 2012-2016 Jacob R. Lifshay
 * This file is part of Voxels.
 *
 * Voxels is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Voxels is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Voxels; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */

// to regenerate this file, use https://github.com/programmerjake/peg_parser_generator
// command: peg_parser_generator parser_imp.peg

#line 29 "javascript_tasklets/parser/parser_imp.cpp"
#include "parser_imp.h"

#line 94 "javascript_tasklets/parser/parser_imp.peg"
             
#include "../character_properties.h"
#include "ast/expression_this.h"
#include "ast/identifier_reference.h"
#include "ast/comma_operator.h"
#include "ast/assignment_expression.h"
#include "../value.h"

#line 41 "javascript_tasklets/parser/parser_imp.cpp"
namespace javascript_tasklets
{
namespace parser
{
Parser::Parser(std::shared_ptr<const char32_t> source, std::size_t sourceSize)
    : resultsPointers(sourceSize, nullptr),
      resultsChunks(),
      eofResults(),
      source(std::move(source)),
      sourceSize(sourceSize)
{
}

Parser::Parser(std::u32string source) : Parser(makeSource(std::move(source)))
{
}

Parser::Parser(const char *source, std::size_t sourceSize) : Parser(makeSource(source, sourceSize))
{
}

Parser::Parser(const char32_t *source, std::size_t sourceSize)
    : Parser(makeSource(std::u32string(source, sourceSize)))
{
}

std::pair<std::shared_ptr<const char32_t>, std::size_t> Parser::makeSource(std::u32string source)
{
    auto sourceSize = source.size();
    auto pSource = std::make_shared<std::u32string>(std::move(source));
    return std::make_pair(std::shared_ptr<const char32_t>(pSource, pSource->data()), sourceSize);
}

std::pair<std::shared_ptr<const char32_t>, std::size_t> Parser::makeSource(const char *source,
                                                                           std::size_t sourceSize)
{
    std::u32string retval;
    retval.reserve(sourceSize);
    std::size_t position = 0;
    const char32_t replacementChar = U'\uFFFD';
    while(position < sourceSize)
    {
        unsigned long byte1 = source[position++];
        if(byte1 < 0x80)
        {
            retval += static_cast<char32_t>(byte1);
            continue;
        }
        if(position >= sourceSize || byte1 < 0xC0 || (source[position] & 0xC0) != 0x80)
        {
            retval += replacementChar;
            continue;
        }
        bool invalid = byte1 < 0xC2 || byte1 > 0xF4;
        unsigned long byte2 = source[position++];
        if(byte1 < 0xE0)
        {
            if(invalid)
                retval += replacementChar;
            else
                retval += static_cast<char32_t>(((byte1 & 0x1F) << 6) | (byte2 & 0x3F));
            continue;
        }
        if(position >= sourceSize || (source[position] & 0xC0) != 0x80)
        {
            retval += replacementChar;
            continue;
        }
        unsigned long byte3 = source[position++];
        if(byte1 < 0xF0)
        {
            if(byte1 == 0xE0 && byte2 < 0xA0)
                invalid = true;
            if(invalid)
                retval += replacementChar;
            else
                retval += static_cast<char32_t>(((byte1 & 0xF) << 12) | ((byte2 & 0x3F) << 6)
                                                | (byte3 & 0x3F));
            continue;
        }
        if(position >= sourceSize || (source[position] & 0xC0) != 0x80)
        {
            retval += replacementChar;
            continue;
        }
        unsigned long byte4 = source[position++];
        if(byte1 == 0xF0 && byte2 < 0x90)
            invalid = true;
        if(byte1 == 0xF4 && byte2 > 0x8F)
            invalid = true;
        if(byte1 > 0xF4)
            invalid = true;
        if(invalid)
            retval += replacementChar;
        else
            retval += static_cast<char32_t>(((byte1 & 0x7) << 18) | ((byte2 & 0x3F) << 12)
                                            | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F));
    }
    return makeSource(std::move(retval));
}

unsigned Parser::parseTokenizerHexDigitValue()
{
    RuleResult result;
    auto retval = internalParseTokenizerHexDigitValue(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

unsigned Parser::internalParseTokenizerHexDigitValue(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    unsigned returnValue__{};
    char32_t ch{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerHexDigitValue;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
    }
    else
    {
        bool matches = false;
        if(!matches)
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            ch = this->source.get()[startLocation__];
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
        }
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 117 "javascript_tasklets/parser/parser_imp.peg"
      
        auto digitValue = character_properties::javascriptDigitValue(ch);
        if(digitValue >= 0 && digitValue < 0x10)
            (returnValue__) = digitValue;
        else
            (predicateReturnValue__) = "missing hex digit";
    
#line 195 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

char32_t Parser::parseTokenizerLineTerminatorSequence()
{
    RuleResult result;
    auto retval = internalParseTokenizerLineTerminatorSequence(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

char32_t Parser::internalParseTokenizerLineTerminatorSequence(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    char32_t returnValue__{};
    char32_t char1{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerLineTerminatorSequence;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing end of line (\'\\r\')", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'\r')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing end of line (\'\\r\')", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing end of line (\'\\n\')", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing end of line (\'\\n\')", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 125 "javascript_tasklets/parser/parser_imp.peg"
                                               (returnValue__) = U'\n';
#line 264 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing end of line (\'\\r\')", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing end of line (\'\\r\')", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 126 "javascript_tasklets/parser/parser_imp.peg"
                                             (returnValue__) = U'\n';
#line 291 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                char1 = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
                const char *predicateReturnValue__ = nullptr;
                {
#line 128 "javascript_tasklets/parser/parser_imp.peg"
                                       
                                         (returnValue__) = char1;
                                         if(!character_properties::javascriptLineTerminator((returnValue__)))
                                             (predicateReturnValue__) = "expected a line terminator";
                                     
#line 337 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                if(predicateReturnValue__ != nullptr)
                    ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool lineTerminatorAllowed>
void Parser::parseTokenizerComment()
{
    RuleResult result;
    internalParseTokenizerComment<lineTerminatorAllowed>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

template <bool lineTerminatorAllowed>
void Parser::internalParseTokenizerComment(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerMultiLineComment<lineTerminatorAllowed>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerSingleLineComment(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
}

template <bool lineTerminatorAllowed>
bool Parser::parseTokenizerMultiLineComment()
{
    RuleResult result;
    auto retval = internalParseTokenizerMultiLineComment<lineTerminatorAllowed>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool lineTerminatorAllowed>
bool Parser::internalParseTokenizerMultiLineComment(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    bool returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerMultiLineComment[static_cast<std::size_t>(lineTerminatorAllowed)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'/')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing *", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'*')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 140 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = false;
#line 449 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                isRequiredForSuccess__ = !isRequiredForSuccess__;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "missing *", isRequiredForSuccess__);
                }
                else if(this->source.get()[startLocation__] == U'*')
                {
                    ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                }
                else
                {
                    ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *", isRequiredForSuccess__);
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    if(startLocation__ >= this->sourceSize)
                    {
                        ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
                    }
                    else if(this->source.get()[startLocation__] == U'/')
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
                    }
                    startLocation__ = savedStartLocation__;
                }
                isRequiredForSuccess__ = !isRequiredForSuccess__;
                if(ruleResult__.success())
                    ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
                else
                    ruleResult__ = this->makeSuccess(startLocation__);
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    if(ruleResult__.success())
                    {
                        auto savedStartLocation__ = startLocation__;
                        startLocation__ = ruleResult__.location;
                        {
                            const char *predicateReturnValue__ = nullptr;
                            {
#line 145 "javascript_tasklets/parser/parser_imp.peg"
              
                if(!lineTerminatorAllowed)
                    (predicateReturnValue__) = "line terminator not allowed here";
                (returnValue__) = true;
            
#line 522 "javascript_tasklets/parser/parser_imp.cpp"
                            }
                            ruleResult__ = this->makeSuccess(startLocation__);
                            if(predicateReturnValue__ != nullptr)
                                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                        }
                        startLocation__ = savedStartLocation__;
                    }
                    if(ruleResult__.fail())
                    {
                        Parser::RuleResult lastRuleResult__ = ruleResult__;
                        isRequiredForSuccess__ = !isRequiredForSuccess__;
                        ruleResult__ = Parser::RuleResult();
                        this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
                        assert(!ruleResult__.empty());
                        isRequiredForSuccess__ = !isRequiredForSuccess__;
                        if(ruleResult__.success())
                            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
                        else
                            ruleResult__ = this->makeSuccess(startLocation__);
                        if(ruleResult__.success())
                        {
                            auto savedStartLocation__ = startLocation__;
                            startLocation__ = ruleResult__.location;
                            if(startLocation__ >= this->sourceSize)
                            {
                                ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                            }
                            else
                            {
                                bool matches = false;
                                if(!matches)
                                {
                                    ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                                }
                                else
                                {
                                    ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                                }
                            }
                            startLocation__ = savedStartLocation__;
                        }
                        if(ruleResult__.success())
                        {
                            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                            {
                                ruleResult__.endLocation = lastRuleResult__.endLocation;
                            }
                        }
                    }
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing *", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'*')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            if(startLocation__ >= this->sourceSize)
            {
                ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
            }
            else if(this->source.get()[startLocation__] == U'/')
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
            }
            startLocation__ = savedStartLocation__;
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

void Parser::parseTokenizerSingleLineComment()
{
    RuleResult result;
    internalParseTokenizerSingleLineComment(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerSingleLineComment(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerSingleLineComment;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'/')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'/')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                isRequiredForSuccess__ = !isRequiredForSuccess__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                isRequiredForSuccess__ = !isRequiredForSuccess__;
                if(ruleResult__.success())
                    ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
                else
                    ruleResult__ = this->makeSuccess(startLocation__);
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    if(startLocation__ >= this->sourceSize)
                    {
                        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                    }
                    else
                    {
                        bool matches = false;
                        if(!matches)
                        {
                            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        }
                        else
                        {
                            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                        }
                    }
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

template <bool lineTerminatorAllowed>
void Parser::parseTokenizerTokenSeperator()
{
    RuleResult result;
    internalParseTokenizerTokenSeperator<lineTerminatorAllowed>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

template <bool lineTerminatorAllowed>
void Parser::internalParseTokenizerTokenSeperator(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    char32_t ch{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTokenSeperator[static_cast<std::size_t>(lineTerminatorAllowed)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    {
        auto savedStartLocation__ = startLocation__;
        auto &savedRuleResult__ = ruleResult__;
        while(true)
        {
            Parser::RuleResult ruleResult__;
            startLocation__ = savedRuleResult__.location;
            ruleResult__ = Parser::RuleResult();
            this->internalParseTokenizerMultiLineComment<lineTerminatorAllowed>(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerSingleLineComment(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
                        const char *predicateReturnValue__ = nullptr;
                        {
#line 163 "javascript_tasklets/parser/parser_imp.peg"
          
            if(!lineTerminatorAllowed)
                (predicateReturnValue__) = "line terminator not allowed here";
        
#line 792 "javascript_tasklets/parser/parser_imp.cpp"
                        }
                        ruleResult__ = this->makeSuccess(startLocation__);
                        if(predicateReturnValue__ != nullptr)
                            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                    }
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                isRequiredForSuccess__ = !isRequiredForSuccess__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                isRequiredForSuccess__ = !isRequiredForSuccess__;
                if(ruleResult__.success())
                    ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
                else
                    ruleResult__ = this->makeSuccess(startLocation__);
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    if(startLocation__ >= this->sourceSize)
                    {
                        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                    }
                    else
                    {
                        bool matches = false;
                        if(!matches)
                        {
                            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                            ch = this->source.get()[startLocation__];
                        }
                        else
                        {
                            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                        }
                    }
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
                        const char *predicateReturnValue__ = nullptr;
                        {
#line 169 "javascript_tasklets/parser/parser_imp.peg"
          
            if(!character_properties::javascriptWhiteSpace(ch))
                (predicateReturnValue__) = "not a whitespace character";
        
#line 855 "javascript_tasklets/parser/parser_imp.cpp"
                        }
                        ruleResult__ = this->makeSuccess(startLocation__);
                        if(predicateReturnValue__ != nullptr)
                            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                    }
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail() || ruleResult__.location == startLocation__)
            {
                savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                startLocation__ = savedStartLocation__;
                break;
            }
            savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
        }
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerTokenSeperatorWithLineTerminator()
{
    RuleResult result;
    internalParseTokenizerTokenSeperatorWithLineTerminator(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerTokenSeperatorWithLineTerminator(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    bool hasLineTerminator{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTokenSeperatorWithLineTerminator;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<false>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        hasLineTerminator = this->internalParseTokenizerMultiLineComment<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
                const char *predicateReturnValue__ = nullptr;
                {
#line 179 "javascript_tasklets/parser/parser_imp.peg"
          
            if(!hasLineTerminator)
                (predicateReturnValue__) = "missing line terminator";
        
#line 923 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                if(predicateReturnValue__ != nullptr)
                    ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.fail())
        {
            Parser::RuleResult lastRuleResult__ = ruleResult__;
            ruleResult__ = Parser::RuleResult();
            this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.success())
            {
                if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                {
                    ruleResult__.endLocation = lastRuleResult__.endLocation;
                }
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

RawStringAndChar Parser::parseTokenizerUnicodeEscapeSequence()
{
    RuleResult result;
    auto retval = internalParseTokenizerUnicodeEscapeSequence(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawStringAndChar Parser::internalParseTokenizerUnicodeEscapeSequence(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawStringAndChar returnValue__{};
    char32_t digit{};
    char32_t digit1{};
    char32_t digit2{};
    char32_t digit3{};
    char32_t digit4{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerUnicodeEscapeSequence;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'u')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 187 "javascript_tasklets/parser/parser_imp.peg"
                                                       (returnValue__) = RawStringAndChar(u"u", 0);
#line 993 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing {", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'{')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing {", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 189 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__).raw += u"{";
#line 1021 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            if(startLocation__ >= this->sourceSize)
            {
                ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
            }
            else
            {
                bool matches = false;
                if(!matches)
                {
                    ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                    digit = this->source.get()[startLocation__];
                }
                else
                {
                    ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                }
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
                    const char *predicateReturnValue__ = nullptr;
                    {
#line 192 "javascript_tasklets/parser/parser_imp.peg"
          
            auto digitValue = character_properties::javascriptDigitValue(digit);
            if(digitValue < 0 || digitValue >= 0x10)
                (predicateReturnValue__) = "missing hex digit";
            else
            {
                (returnValue__).ch = (returnValue__).ch * 0x10 + digitValue;
                (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), digit);
            }
            if((returnValue__).ch > 0x10FFFF)
                (predicateReturnValue__) = "unicode escape value is too big";
        
#line 1067 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    if(predicateReturnValue__ != nullptr)
                        ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                }
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                auto &savedRuleResult__ = ruleResult__;
                while(true)
                {
                    Parser::RuleResult ruleResult__;
                    startLocation__ = savedRuleResult__.location;
                    if(startLocation__ >= this->sourceSize)
                    {
                        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                    }
                    else
                    {
                        bool matches = false;
                        if(!matches)
                        {
                            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                            digit = this->source.get()[startLocation__];
                        }
                        else
                        {
                            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                        }
                    }
                    if(ruleResult__.success())
                    {
                        auto savedStartLocation__ = startLocation__;
                        startLocation__ = ruleResult__.location;
                        {
                            const char *predicateReturnValue__ = nullptr;
                            {
#line 192 "javascript_tasklets/parser/parser_imp.peg"
          
            auto digitValue = character_properties::javascriptDigitValue(digit);
            if(digitValue < 0 || digitValue >= 0x10)
                (predicateReturnValue__) = "missing hex digit";
            else
            {
                (returnValue__).ch = (returnValue__).ch * 0x10 + digitValue;
                (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), digit);
            }
            if((returnValue__).ch > 0x10FFFF)
                (predicateReturnValue__) = "unicode escape value is too big";
        
#line 1120 "javascript_tasklets/parser/parser_imp.cpp"
                            }
                            ruleResult__ = this->makeSuccess(startLocation__);
                            if(predicateReturnValue__ != nullptr)
                                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                        }
                        startLocation__ = savedStartLocation__;
                    }
                    if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                    {
                        savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                        startLocation__ = savedStartLocation__;
                        break;
                    }
                    savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
                }
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            if(startLocation__ >= this->sourceSize)
            {
                ruleResult__ = this->makeFail(startLocation__, "missing }", isRequiredForSuccess__);
            }
            else if(this->source.get()[startLocation__] == U'}')
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing }", isRequiredForSuccess__);
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 206 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__).raw += u"}";
#line 1164 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.fail())
        {
            Parser::RuleResult lastRuleResult__ = ruleResult__;
            if(startLocation__ >= this->sourceSize)
            {
                ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
            }
            else
            {
                bool matches = false;
                if(!matches)
                {
                    ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                    digit1 = this->source.get()[startLocation__];
                }
                else
                {
                    ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                }
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
                    const char *predicateReturnValue__ = nullptr;
                    {
#line 208 "javascript_tasklets/parser/parser_imp.peg"
        
          auto digitValue = character_properties::javascriptDigitValue(digit1);
          if(digitValue < 0 || digitValue >= 0x10)
              (predicateReturnValue__) = "missing hex digit";
          else
          {
              (returnValue__).ch = (returnValue__).ch * 0x10 + digitValue;
              (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), digit1);
          }
      
#line 1207 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    if(predicateReturnValue__ != nullptr)
                        ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                }
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                }
                else
                {
                    bool matches = false;
                    if(!matches)
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        digit2 = this->source.get()[startLocation__];
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                    }
                }
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
                    const char *predicateReturnValue__ = nullptr;
                    {
#line 219 "javascript_tasklets/parser/parser_imp.peg"
        
          auto digitValue = character_properties::javascriptDigitValue(digit2);
          if(digitValue < 0 || digitValue >= 0x10)
              (predicateReturnValue__) = "missing hex digit";
          else
          {
              (returnValue__).ch = (returnValue__).ch * 0x10 + digitValue;
              (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), digit2);
          }
      
#line 1256 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    if(predicateReturnValue__ != nullptr)
                        ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                }
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                }
                else
                {
                    bool matches = false;
                    if(!matches)
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        digit3 = this->source.get()[startLocation__];
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                    }
                }
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
                    const char *predicateReturnValue__ = nullptr;
                    {
#line 230 "javascript_tasklets/parser/parser_imp.peg"
        
          auto digitValue = character_properties::javascriptDigitValue(digit3);
          if(digitValue < 0 || digitValue >= 0x10)
              (predicateReturnValue__) = "missing hex digit";
          else
          {
              (returnValue__).ch = (returnValue__).ch * 0x10 + digitValue;
              (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), digit3);
          }
      
#line 1305 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    if(predicateReturnValue__ != nullptr)
                        ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                }
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                }
                else
                {
                    bool matches = false;
                    if(!matches)
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        digit4 = this->source.get()[startLocation__];
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                    }
                }
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
                    const char *predicateReturnValue__ = nullptr;
                    {
#line 241 "javascript_tasklets/parser/parser_imp.peg"
        
          auto digitValue = character_properties::javascriptDigitValue(digit4);
          if(digitValue < 0 || digitValue >= 0x10)
              (predicateReturnValue__) = "missing hex digit";
          else
          {
              (returnValue__).ch = (returnValue__).ch * 0x10 + digitValue;
              (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), digit4);
          }
      
#line 1354 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    if(predicateReturnValue__ != nullptr)
                        ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                }
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                {
                    ruleResult__.endLocation = lastRuleResult__.endLocation;
                }
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

char32_t Parser::parseTokenizerUnicodeEscapeOrChar()
{
    RuleResult result;
    auto retval = internalParseTokenizerUnicodeEscapeOrChar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

char32_t Parser::internalParseTokenizerUnicodeEscapeOrChar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    char32_t returnValue__{};
    RawStringAndChar escape{};
    char32_t ch{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerUnicodeEscapeOrChar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'\\')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        escape = this->internalParseTokenizerUnicodeEscapeSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 253 "javascript_tasklets/parser/parser_imp.peg"
                                                                                (returnValue__) = escape.ch;
#line 1425 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                ch = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 254 "javascript_tasklets/parser/parser_imp.peg"
                                            (returnValue__) = ch;
#line 1457 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

char32_t Parser::parseTokenizerEscapelessIdentifierStart()
{
    RuleResult result;
    auto retval = internalParseTokenizerEscapelessIdentifierStart(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

char32_t Parser::internalParseTokenizerEscapelessIdentifierStart(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    char32_t returnValue__{};
    char32_t ch{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEscapelessIdentifierStart;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
    }
    else
    {
        bool matches = false;
        if(!matches)
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            ch = this->source.get()[startLocation__];
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
        }
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 258 "javascript_tasklets/parser/parser_imp.peg"
      
        if(!character_properties::javascriptIdStart(ch))
            (predicateReturnValue__) = "expected identifier start";
        else
            (returnValue__) = ch;
    
#line 1525 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

char32_t Parser::parseTokenizerEscapelessIdentifierPart()
{
    RuleResult result;
    auto retval = internalParseTokenizerEscapelessIdentifierPart(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

char32_t Parser::internalParseTokenizerEscapelessIdentifierPart(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    char32_t returnValue__{};
    char32_t ch{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEscapelessIdentifierPart;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
    }
    else
    {
        bool matches = false;
        if(!matches)
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            ch = this->source.get()[startLocation__];
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
        }
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 267 "javascript_tasklets/parser/parser_imp.peg"
      
        if(!character_properties::javascriptIdContinue(ch))
            (predicateReturnValue__) = "expected identifier part";
        else
            (returnValue__) = ch;
    
#line 1588 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerIdentifierName()
{
    RuleResult result;
    auto retval = internalParseTokenizerIdentifierName(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerIdentifierName(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t startChar{};
    char32_t continueChar{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerIdentifierName;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    startChar = this->internalParseTokenizerUnicodeEscapeOrChar(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 276 "javascript_tasklets/parser/parser_imp.peg"
      
        if(!character_properties::javascriptIdStart(startChar))
            (predicateReturnValue__) = "expected identifier start";
        else
            (returnValue__) = startChar;
    
#line 1638 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                continueChar = this->internalParseTokenizerUnicodeEscapeOrChar(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
                        const char *predicateReturnValue__ = nullptr;
                        {
#line 284 "javascript_tasklets/parser/parser_imp.peg"
          
            if(!character_properties::javascriptIdContinue(continueChar))
                (predicateReturnValue__) = "expected identifier continue";
            else
                (returnValue__) = continueChar;
        
#line 1675 "javascript_tasklets/parser/parser_imp.cpp"
                        }
                        ruleResult__ = this->makeSuccess(startLocation__);
                        if(predicateReturnValue__ != nullptr)
                            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                    }
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerEscapelessIdentifierName()
{
    RuleResult result;
    auto retval = internalParseTokenizerEscapelessIdentifierName(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerEscapelessIdentifierName(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t startChar{};
    char32_t continueChar{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEscapelessIdentifierName;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
    }
    else
    {
        bool matches = false;
        if(!matches)
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            startChar = this->source.get()[startLocation__];
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
        }
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 295 "javascript_tasklets/parser/parser_imp.peg"
      
        if(!character_properties::javascriptIdStart(startChar))
            (predicateReturnValue__) = "expected identifier start";
        else
            (returnValue__) = appendCodePoint(u"", startChar);
    
#line 1774 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                }
                else
                {
                    bool matches = false;
                    if(!matches)
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        continueChar = this->source.get()[startLocation__];
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
                    }
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
                        const char *predicateReturnValue__ = nullptr;
                        {
#line 303 "javascript_tasklets/parser/parser_imp.peg"
          
            if(!character_properties::javascriptIdContinue(continueChar))
                (predicateReturnValue__) = "expected identifier continue";
            else
                (returnValue__) = appendCodePoint(std::move((returnValue__)), continueChar);
        
#line 1825 "javascript_tasklets/parser/parser_imp.cpp"
                        }
                        ruleResult__ = this->makeSuccess(startLocation__);
                        if(predicateReturnValue__ != nullptr)
                            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
                    }
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict>
void Parser::parseTokenizerReservedWord()
{
    RuleResult result;
    internalParseTokenizerReservedWord<isModule, isStrict>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

template <bool isModule, bool isStrict>
void Parser::internalParseTokenizerReservedWord(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerKeyword(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFutureReservedWord<isModule, isStrict>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerNullLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerBooleanLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerAwait()
{
    RuleResult result;
    internalParseTokenizerAwait(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerAwait(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerAwait;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'a')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing w", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'w')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing w", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerBreak()
{
    RuleResult result;
    internalParseTokenizerBreak(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerBreak(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerBreak;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing b", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'b')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing b", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing k", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'k')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing k", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerCase()
{
    RuleResult result;
    internalParseTokenizerCase(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerCase(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerCase;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'c')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerCatch()
{
    RuleResult result;
    internalParseTokenizerCatch(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerCatch(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerCatch;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'c')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'c')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing h", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'h')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing h", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerClass()
{
    RuleResult result;
    internalParseTokenizerClass(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerClass(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerClass;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'c')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerConst()
{
    RuleResult result;
    internalParseTokenizerConst(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerConst(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerConst;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'c')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerContinue()
{
    RuleResult result;
    internalParseTokenizerContinue(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerContinue(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerContinue;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'c')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerDebugger()
{
    RuleResult result;
    internalParseTokenizerDebugger(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerDebugger(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDebugger;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing d", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'd')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing d", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing b", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'b')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing b", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing g", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'g')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing g", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing g", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'g')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing g", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerDefault()
{
    RuleResult result;
    internalParseTokenizerDefault(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerDefault(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDefault;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing d", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'd')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing d", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'f')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerDelete()
{
    RuleResult result;
    internalParseTokenizerDelete(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerDelete(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDelete;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing d", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'd')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing d", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerDo()
{
    RuleResult result;
    internalParseTokenizerDo(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerDo(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDo;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing d", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'd')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing d", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerElse()
{
    RuleResult result;
    internalParseTokenizerElse(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerElse(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerElse;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'e')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerEnum()
{
    RuleResult result;
    internalParseTokenizerEnum(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerEnum(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEnum;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'e')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing m", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'm')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing m", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerExport()
{
    RuleResult result;
    internalParseTokenizerExport(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerExport(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerExport;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'e')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing x", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'x')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing x", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'p')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerExtends()
{
    RuleResult result;
    internalParseTokenizerExtends(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerExtends(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerExtends;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'e')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing x", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'x')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing x", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing d", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'd')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing d", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerFalse()
{
    RuleResult result;
    internalParseTokenizerFalse(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerFalse(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerFalse;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'f')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerFinally()
{
    RuleResult result;
    internalParseTokenizerFinally(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerFinally(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerFinally;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'f')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing y", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'y')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing y", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerFor()
{
    RuleResult result;
    internalParseTokenizerFor(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerFor(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerFor;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'f')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerFunction()
{
    RuleResult result;
    internalParseTokenizerFunction(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerFunction(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerFunction;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'f')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'c')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerIf()
{
    RuleResult result;
    internalParseTokenizerIf(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerIf(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerIf;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'i')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'f')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerImplements()
{
    RuleResult result;
    internalParseTokenizerImplements(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerImplements(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerImplements;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'i')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing m", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'm')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing m", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'p')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing m", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'm')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing m", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerImport()
{
    RuleResult result;
    internalParseTokenizerImport(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerImport(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerImport;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'i')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing m", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'm')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing m", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'p')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerIn()
{
    RuleResult result;
    internalParseTokenizerIn(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerIn(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerIn;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'i')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerInstanceOf()
{
    RuleResult result;
    internalParseTokenizerInstanceOf(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerInstanceOf(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerInstanceOf;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'i')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'c')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'f')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerInterface()
{
    RuleResult result;
    internalParseTokenizerInterface(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerInterface(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerInterface;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'i')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'f')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'c')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerNew()
{
    RuleResult result;
    internalParseTokenizerNew(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerNew(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerNew;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'n')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing w", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'w')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing w", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerNull()
{
    RuleResult result;
    internalParseTokenizerNull(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerNull(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerNull;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'n')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPackage()
{
    RuleResult result;
    internalParseTokenizerPackage(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPackage(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPackage;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'p')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'c')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing k", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'k')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing k", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing g", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'g')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing g", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPrivate()
{
    RuleResult result;
    internalParseTokenizerPrivate(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPrivate(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPrivate;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'p')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing v", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'v')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing v", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerProtected()
{
    RuleResult result;
    internalParseTokenizerProtected(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerProtected(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerProtected;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'p')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'c')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing d", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'd')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing d", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPublic()
{
    RuleResult result;
    internalParseTokenizerPublic(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPublic(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPublic;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'p')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing b", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'b')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing b", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'c')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerReturn()
{
    RuleResult result;
    internalParseTokenizerReturn(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerReturn(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerReturn;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'r')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerSuper()
{
    RuleResult result;
    internalParseTokenizerSuper(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerSuper(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerSuper;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U's')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'p')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerSwitch()
{
    RuleResult result;
    internalParseTokenizerSwitch(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerSwitch(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerSwitch;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U's')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing w", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'w')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing w", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing c", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'c')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing c", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing h", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'h')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing h", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerThis()
{
    RuleResult result;
    internalParseTokenizerThis(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerThis(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerThis;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U't')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing h", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'h')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing h", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing s", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U's')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing s", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerThrow()
{
    RuleResult result;
    internalParseTokenizerThrow(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerThrow(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerThrow;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U't')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing h", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'h')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing h", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing w", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'w')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing w", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerTrue()
{
    RuleResult result;
    internalParseTokenizerTrue(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerTrue(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTrue;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U't')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing u", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'u')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing u", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerTry()
{
    RuleResult result;
    internalParseTokenizerTry(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerTry(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTry;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U't')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing y", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'y')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing y", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerTypeOf()
{
    RuleResult result;
    internalParseTokenizerTypeOf(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerTypeOf(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTypeOf;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U't')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing y", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'y')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing y", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing p", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'p')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing p", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'f')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerVar()
{
    RuleResult result;
    internalParseTokenizerVar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerVar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerVar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing v", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'v')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing v", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing a", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'a')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing a", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerVoid()
{
    RuleResult result;
    internalParseTokenizerVoid(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerVoid(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerVoid;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing v", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'v')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing v", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing o", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'o')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing o", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing d", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'd')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing d", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerWhile()
{
    RuleResult result;
    internalParseTokenizerWhile(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerWhile(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerWhile;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing w", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'w')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing w", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing h", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'h')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing h", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerWith()
{
    RuleResult result;
    internalParseTokenizerWith(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerWith(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerWith;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing w", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'w')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing w", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing h", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'h')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing h", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerYield()
{
    RuleResult result;
    internalParseTokenizerYield(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerYield(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerYield;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing y", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'y')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing y", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing i", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'i')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing i", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing e", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing e", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing l", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'l')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing l", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing d", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'd')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing d", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerKeyword()
{
    RuleResult result;
    internalParseTokenizerKeyword(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerKeyword(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerBreak(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerCase(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerCatch(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerClass(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerConst(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerContinue(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerDebugger(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerDefault(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerDelete(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerDo(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerElse(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerExport(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerExtends(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFinally(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFor(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFunction(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerIf(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerImport(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerIn(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerInstanceOf(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerNew(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerReturn(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerSuper(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerSwitch(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerThis(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerThrow(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerTry(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerTypeOf(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerVar(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerVoid(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerWhile(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerWith(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerYield(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
}

template <bool isModule, bool isStrict>
void Parser::parseTokenizerFutureReservedWord()
{
    RuleResult result;
    internalParseTokenizerFutureReservedWord<isModule, isStrict>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

template <bool isModule, bool isStrict>
void Parser::internalParseTokenizerFutureReservedWord(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerFutureReservedWord[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerAwait(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 394 "javascript_tasklets/parser/parser_imp.peg"
                     if(isModule) (predicateReturnValue__) = "not in a module";
#line 9065 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEnum(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 396 "javascript_tasklets/parser/parser_imp.peg"
        if(!isStrict) (predicateReturnValue__) = "not strict";
#line 9095 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            this->internalParseTokenizerImplements(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerInterface(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerPackage(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerPrivate(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerProtected(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenizerPublic(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerNullLiteral()
{
    RuleResult result;
    internalParseTokenizerNullLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerNullLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerNull(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    ruleResultOut__ = ruleResult__;
}

bool Parser::parseTokenizerBooleanLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerBooleanLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

bool Parser::internalParseTokenizerBooleanLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    bool returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerBooleanLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTrue(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 408 "javascript_tasklets/parser/parser_imp.peg"
                                              (returnValue__) = true;
#line 9238 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFalse(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 408 "javascript_tasklets/parser/parser_imp.peg"
                                                                            (returnValue__) = false;
#line 9256 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

void Parser::parseTokenizerLBrace()
{
    RuleResult result;
    internalParseTokenizerLBrace(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerLBrace(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing {", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'{')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing {", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRBrace()
{
    RuleResult result;
    internalParseTokenizerRBrace(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRBrace(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing }", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'}')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing }", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerLParen()
{
    RuleResult result;
    internalParseTokenizerLParen(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerLParen(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing (", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'(')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing (", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRParen()
{
    RuleResult result;
    internalParseTokenizerRParen(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRParen(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing )", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U')')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing )", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerLBracket()
{
    RuleResult result;
    internalParseTokenizerLBracket(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerLBracket(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing [", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'[')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing [", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRBracket()
{
    RuleResult result;
    internalParseTokenizerRBracket(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRBracket(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing ]", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U']')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing ]", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPeriod()
{
    RuleResult result;
    internalParseTokenizerPeriod(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPeriod(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPeriod;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'.')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'.')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'.')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(ruleResult__.success())
        ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
    else
        ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'.')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'9')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing decimal digit", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerEllipsis()
{
    RuleResult result;
    internalParseTokenizerEllipsis(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerEllipsis(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEllipsis;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'.')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'.')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'.')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerSemicolon()
{
    RuleResult result;
    internalParseTokenizerSemicolon(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerSemicolon(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing ;", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U';')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing ;", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerComma()
{
    RuleResult result;
    internalParseTokenizerComma(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerComma(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing ,", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U',')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing ,", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerLAngle()
{
    RuleResult result;
    internalParseTokenizerLAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerLAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerLAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing <", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'<')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing <", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'<')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing < or =", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRAngle()
{
    RuleResult result;
    internalParseTokenizerRAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'>')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'>')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing = or >", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerLAngleEqual()
{
    RuleResult result;
    internalParseTokenizerLAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerLAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerLAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing <", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'<')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing <", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRAngleEqual()
{
    RuleResult result;
    internalParseTokenizerRAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'>')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerEqualEqual()
{
    RuleResult result;
    internalParseTokenizerEqualEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerEqualEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEqualEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'=')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerEMarkEqual()
{
    RuleResult result;
    internalParseTokenizerEMarkEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerEMarkEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEMarkEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing !", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'!')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing !", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerEqualEqualEqual()
{
    RuleResult result;
    internalParseTokenizerEqualEqualEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerEqualEqualEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEqualEqualEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'=')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerEMarkEqualEqual()
{
    RuleResult result;
    internalParseTokenizerEMarkEqualEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerEMarkEqualEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEMarkEqualEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing !", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'!')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing !", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPlus()
{
    RuleResult result;
    internalParseTokenizerPlus(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPlus(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPlus;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing +", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'+')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing +", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'+')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing + or =", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerMinus()
{
    RuleResult result;
    internalParseTokenizerMinus(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerMinus(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerMinus;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing -", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'-')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing -", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'-')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing - or =", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerStar()
{
    RuleResult result;
    internalParseTokenizerStar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerStar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerStar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing *", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'*')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'*')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing * or =", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerStarStar()
{
    RuleResult result;
    internalParseTokenizerStarStar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerStarStar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerStarStar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing *", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'*')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing *", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'*')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPercent()
{
    RuleResult result;
    internalParseTokenizerPercent(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPercent(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPercent;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing %", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'%')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing %", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPlusPlus()
{
    RuleResult result;
    internalParseTokenizerPlusPlus(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPlusPlus(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPlusPlus;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing +", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'+')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing +", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing +", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'+')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing +", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerMinusMinus()
{
    RuleResult result;
    internalParseTokenizerMinusMinus(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerMinusMinus(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerMinusMinus;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing -", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'-')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing -", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing -", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'-')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing -", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerLAngleLAngle()
{
    RuleResult result;
    internalParseTokenizerLAngleLAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerLAngleLAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerLAngleLAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing <", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'<')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing <", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing <", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'<')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing <", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRAngleRAngle()
{
    RuleResult result;
    internalParseTokenizerRAngleRAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRAngleRAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRAngleRAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'>')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'>')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'>')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing = or >", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRAngleRAngleRAngle()
{
    RuleResult result;
    internalParseTokenizerRAngleRAngleRAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRAngleRAngleRAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRAngleRAngleRAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'>')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'>')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'>')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerAmp()
{
    RuleResult result;
    internalParseTokenizerAmp(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerAmp(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerAmp;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing &", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'&')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing &", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'&')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing & or =", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPipe()
{
    RuleResult result;
    internalParseTokenizerPipe(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPipe(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPipe;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing |", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'|')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing |", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'|')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing = or |", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerCaret()
{
    RuleResult result;
    internalParseTokenizerCaret(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerCaret(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerCaret;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing ^", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'^')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing ^", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerEMark()
{
    RuleResult result;
    internalParseTokenizerEMark(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerEMark(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEMark;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing !", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'!')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing !", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerTilde()
{
    RuleResult result;
    internalParseTokenizerTilde(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerTilde(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing ~", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'~')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing ~", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerAmpAmp()
{
    RuleResult result;
    internalParseTokenizerAmpAmp(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerAmpAmp(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerAmpAmp;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing &", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'&')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing &", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing &", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'&')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing &", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPipePipe()
{
    RuleResult result;
    internalParseTokenizerPipePipe(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPipePipe(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPipePipe;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing |", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'|')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing |", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing |", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'|')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing |", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerQMark()
{
    RuleResult result;
    internalParseTokenizerQMark(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerQMark(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing \?", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'\?')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \?", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerColon()
{
    RuleResult result;
    internalParseTokenizerColon(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerColon(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing :", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U':')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing :", isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerEqual()
{
    RuleResult result;
    internalParseTokenizerEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'=')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'>')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing = or >", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPlusEqual()
{
    RuleResult result;
    internalParseTokenizerPlusEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPlusEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPlusEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing +", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'+')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing +", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerMinusEqual()
{
    RuleResult result;
    internalParseTokenizerMinusEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerMinusEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerMinusEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing -", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'-')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing -", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerStarEqual()
{
    RuleResult result;
    internalParseTokenizerStarEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerStarEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerStarEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing *", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'*')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPercentEqual()
{
    RuleResult result;
    internalParseTokenizerPercentEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPercentEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPercentEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing %", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'%')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing %", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerLAngleLAngleEqual()
{
    RuleResult result;
    internalParseTokenizerLAngleLAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerLAngleLAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerLAngleLAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing <", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'<')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing <", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing <", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'<')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing <", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRAngleRAngleEqual()
{
    RuleResult result;
    internalParseTokenizerRAngleRAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRAngleRAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRAngleRAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'>')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'>')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRAngleRAngleRAngleEqual()
{
    RuleResult result;
    internalParseTokenizerRAngleRAngleRAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRAngleRAngleRAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRAngleRAngleRAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'>')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'>')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'>')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerAmpEqual()
{
    RuleResult result;
    internalParseTokenizerAmpEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerAmpEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerAmpEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing &", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'&')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing &", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPipeEqual()
{
    RuleResult result;
    internalParseTokenizerPipeEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPipeEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerPipeEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing |", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'|')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing |", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerCaretEqual()
{
    RuleResult result;
    internalParseTokenizerCaretEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerCaretEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerCaretEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing ^", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'^')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing ^", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerArrow()
{
    RuleResult result;
    internalParseTokenizerArrow(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerArrow(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerArrow;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'=')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing >", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'>')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing >", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerFSlash()
{
    RuleResult result;
    internalParseTokenizerFSlash(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerFSlash(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerFSlash;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'/')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'*')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'/')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'=')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *, /, or =", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerFSlashEqual()
{
    RuleResult result;
    internalParseTokenizerFSlashEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerFSlashEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerFSlashEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'/')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing =", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'=')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing =", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerPunctuator()
{
    RuleResult result;
    internalParseTokenizerPunctuator(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerPunctuator(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerLBrace(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLParen(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRParen(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLBracket(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRBracket(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPeriod(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEllipsis(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerSemicolon(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerComma(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEqualEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEMarkEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEqualEqualEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEMarkEqualEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPlus(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerMinus(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerStar(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerStarStar(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPercent(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPlusPlus(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerMinusMinus(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLAngleLAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleRAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleRAngleRAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerAmp(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPipe(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerCaret(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEMark(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerTilde(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerAmpAmp(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPipePipe(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerQMark(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerColon(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPlusEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerMinusEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerStarEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPercentEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLAngleLAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleRAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleRAngleRAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerAmpEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPipeEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerCaretEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerArrow(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerDivPunctuator()
{
    RuleResult result;
    internalParseTokenizerDivPunctuator(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerDivPunctuator(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerFSlash(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFSlashEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenizerRightBracePunctuator()
{
    RuleResult result;
    internalParseTokenizerRightBracePunctuator(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenizerRightBracePunctuator(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    Parser::RuleResult ruleResult__;
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerRBrace(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    ruleResultOut__ = ruleResult__;
}

String Parser::parseTokenizerNumericLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerNumericLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerNumericLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String value0{};
    String value1{};
    String value2{};
    String value3{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerNumericLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    value0 = this->internalParseTokenizerDecimalLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 515 "javascript_tasklets/parser/parser_imp.peg"
                                                                 (returnValue__) = value0;
#line 12863 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        value1 = this->internalParseTokenizerBinaryIntegerLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 516 "javascript_tasklets/parser/parser_imp.peg"
                                                                       (returnValue__) = value1;
#line 12881 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        value2 = this->internalParseTokenizerOctalIntegerLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 517 "javascript_tasklets/parser/parser_imp.peg"
                                                                      (returnValue__) = value2;
#line 12907 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        value3 = this->internalParseTokenizerHexIntegerLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 518 "javascript_tasklets/parser/parser_imp.peg"
                                                                    (returnValue__) = value3;
#line 12933 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerDecimalLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerDecimalLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerDecimalLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String value0{};
    String value1{};
    String value2{};
    String value3{};
    String value4{};
    String value5{};
    String value6{};
    String value7{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDecimalLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    value0 = this->internalParseTokenizerDecimalIntegerLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 520 "javascript_tasklets/parser/parser_imp.peg"
                                                                        (returnValue__) = value0;
#line 12987 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'.')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 521 "javascript_tasklets/parser/parser_imp.peg"
                                      (returnValue__) += u'.';
#line 13017 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value1 = this->internalParseTokenizerDecimalDigits(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 523 "javascript_tasklets/parser/parser_imp.peg"
                                                                    (returnValue__) += value1;
#line 13036 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.fail())
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value2 = this->internalParseTokenizerExponentPart(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 526 "javascript_tasklets/parser/parser_imp.peg"
                                                                   (returnValue__) += value2;
#line 13059 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.fail())
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing .", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'.')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing .", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 528 "javascript_tasklets/parser/parser_imp.peg"
                                      (returnValue__) = u'.';
#line 13090 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            value3 = this->internalParseTokenizerDecimalDigits(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 529 "javascript_tasklets/parser/parser_imp.peg"
                                                                (returnValue__) += value3;
#line 13111 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            value4 = this->internalParseTokenizerExponentPart(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
#line 531 "javascript_tasklets/parser/parser_imp.peg"
                                                                   (returnValue__) += value4;
#line 13130 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.fail())
                ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        value5 = this->internalParseTokenizerDecimalIntegerLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 533 "javascript_tasklets/parser/parser_imp.peg"
                                                                        (returnValue__) = value5;
#line 13160 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            value6 = this->internalParseTokenizerDecimalDigits(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 534 "javascript_tasklets/parser/parser_imp.peg"
                                                                (returnValue__) += value6;
#line 13181 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            value7 = this->internalParseTokenizerExponentPart(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
#line 536 "javascript_tasklets/parser/parser_imp.peg"
                                                                   (returnValue__) += value7;
#line 13200 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.fail())
                ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerDecimalIntegerLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerDecimalIntegerLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerDecimalIntegerLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t nonZeroDigit{};
    String digits{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDecimalIntegerLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing 0", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'0')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing 0", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 541 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = u"0";
#line 13261 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'9')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing decimal digit", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] >= U'1' && this->source.get()[startLocation__] <= U'9')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                nonZeroDigit = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing [1-9]", isRequiredForSuccess__);
            }
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 543 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(u"", nonZeroDigit);
#line 13329 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            digits = this->internalParseTokenizerDecimalDigits(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
#line 544 "javascript_tasklets/parser/parser_imp.peg"
                                    (returnValue__) += digits;
#line 13348 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.fail())
                ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerDecimalDigits()
{
    RuleResult result;
    auto retval = internalParseTokenizerDecimalDigits(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerDecimalDigits(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t digit{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDecimalDigits;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
#line 547 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 13392 "javascript_tasklets/parser/parser_imp.cpp"
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'9')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                digit = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing decimal digit", isRequiredForSuccess__);
            }
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 550 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), digit);
#line 13427 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                }
                else
                {
                    bool matches = false;
                    if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'9')
                    {
                        matches = true;
                    }
                    if(matches)
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        digit = this->source.get()[startLocation__];
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing decimal digit", isRequiredForSuccess__);
                    }
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 550 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), digit);
#line 13468 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerExponentPart()
{
    RuleResult result;
    auto retval = internalParseTokenizerExponentPart(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerExponentPart(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t exponentIndicator{};
    String signedInteger{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerExponentPart;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
    }
    else
    {
        bool matches = false;
        if(this->source.get()[startLocation__] == U'E')
        {
            matches = true;
        }
        else if(this->source.get()[startLocation__] == U'e')
        {
            matches = true;
        }
        if(matches)
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            exponentIndicator = this->source.get()[startLocation__];
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing E or e", isRequiredForSuccess__);
        }
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 555 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(u"", exponentIndicator);
#line 13541 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        signedInteger = this->internalParseTokenizerSignedInteger(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 557 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) += signedInteger;
#line 13562 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerSignedInteger()
{
    RuleResult result;
    auto retval = internalParseTokenizerSignedInteger(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerSignedInteger(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t sign{};
    String digits{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerSignedInteger;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
#line 561 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 13595 "javascript_tasklets/parser/parser_imp.cpp"
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'+')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'-')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                sign = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing + or -", isRequiredForSuccess__);
            }
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 564 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(u"", sign);
#line 13634 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.fail())
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        digits = this->internalParseTokenizerDecimalDigits(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 567 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) += digits;
#line 13659 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerBinaryIntegerLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerBinaryIntegerLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerBinaryIntegerLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t binaryIndicator{};
    String digits{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerBinaryIntegerLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing 0", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'0')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing 0", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 572 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"0";
#line 13708 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'B')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'b')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                binaryIndicator = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing B or b", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 574 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(std::move((returnValue__)), binaryIndicator);
#line 13751 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        digits = this->internalParseTokenizerBinaryDigits(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 576 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) += digits;
#line 13772 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerBinaryDigits()
{
    RuleResult result;
    auto retval = internalParseTokenizerBinaryDigits(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerBinaryDigits(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t digit{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerBinaryDigits;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
#line 580 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 13804 "javascript_tasklets/parser/parser_imp.cpp"
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'0')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'1')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                digit = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing 0 or 1", isRequiredForSuccess__);
            }
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 583 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), digit);
#line 13843 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                }
                else
                {
                    bool matches = false;
                    if(this->source.get()[startLocation__] == U'0')
                    {
                        matches = true;
                    }
                    else if(this->source.get()[startLocation__] == U'1')
                    {
                        matches = true;
                    }
                    if(matches)
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        digit = this->source.get()[startLocation__];
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing 0 or 1", isRequiredForSuccess__);
                    }
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 583 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), digit);
#line 13888 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerOctalIntegerLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerOctalIntegerLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerOctalIntegerLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t octalIndicator{};
    String digits{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerOctalIntegerLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing 0", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'0')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing 0", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 588 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"0";
#line 13948 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'O')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'o')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                octalIndicator = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing O or o", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 590 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(std::move((returnValue__)), octalIndicator);
#line 13991 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        digits = this->internalParseTokenizerOctalDigits(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 592 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) += digits;
#line 14012 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerOctalDigits()
{
    RuleResult result;
    auto retval = internalParseTokenizerOctalDigits(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerOctalDigits(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t digit{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerOctalDigits;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
#line 596 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 14044 "javascript_tasklets/parser/parser_imp.cpp"
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'7')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                digit = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing octal digit", isRequiredForSuccess__);
            }
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 599 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), digit);
#line 14079 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                }
                else
                {
                    bool matches = false;
                    if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'7')
                    {
                        matches = true;
                    }
                    if(matches)
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        digit = this->source.get()[startLocation__];
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing octal digit", isRequiredForSuccess__);
                    }
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 599 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), digit);
#line 14120 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerHexIntegerLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerHexIntegerLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerHexIntegerLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t hexIndicator{};
    String digits{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerHexIntegerLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing 0", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'0')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing 0", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 604 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"0";
#line 14180 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] == U'X')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'x')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                hexIndicator = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing X or x", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 606 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(std::move((returnValue__)), hexIndicator);
#line 14223 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        digits = this->internalParseTokenizerHexDigits(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 608 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) += digits;
#line 14244 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerHexDigits()
{
    RuleResult result;
    auto retval = internalParseTokenizerHexDigits(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerHexDigits(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t digit{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerHexDigits;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
#line 612 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 14276 "javascript_tasklets/parser/parser_imp.cpp"
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'9')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] >= U'A' && this->source.get()[startLocation__] <= U'F')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] >= U'a' && this->source.get()[startLocation__] <= U'f')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                digit = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing hexadecimal digit", isRequiredForSuccess__);
            }
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 615 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), digit);
#line 14319 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                if(startLocation__ >= this->sourceSize)
                {
                    ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
                }
                else
                {
                    bool matches = false;
                    if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'9')
                    {
                        matches = true;
                    }
                    else if(this->source.get()[startLocation__] >= U'A' && this->source.get()[startLocation__] <= U'F')
                    {
                        matches = true;
                    }
                    else if(this->source.get()[startLocation__] >= U'a' && this->source.get()[startLocation__] <= U'f')
                    {
                        matches = true;
                    }
                    if(matches)
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                        digit = this->source.get()[startLocation__];
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing hexadecimal digit", isRequiredForSuccess__);
                    }
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 615 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), digit);
#line 14368 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerStringLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerStringLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerStringLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String characters1{};
    String characters2{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerStringLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing \'", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'\'')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \'", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 620 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 14428 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        characters1 = this->internalParseTokenizerSingleStringCharacters(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 623 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = characters1;
#line 14447 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.fail())
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \'", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\'')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \'", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \"", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\"')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \"", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 627 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 14496 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            characters2 = this->internalParseTokenizerDoubleStringCharacters(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
#line 630 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = characters2;
#line 14515 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.fail())
                ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            if(startLocation__ >= this->sourceSize)
            {
                ruleResult__ = this->makeFail(startLocation__, "missing \"", isRequiredForSuccess__);
            }
            else if(this->source.get()[startLocation__] == U'\"')
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \"", isRequiredForSuccess__);
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerSingleStringCharacters()
{
    RuleResult result;
    auto retval = internalParseTokenizerSingleStringCharacters(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerSingleStringCharacters(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String first{};
    String next{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerSingleStringCharacters;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    first = this->internalParseTokenizerSingleStringCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 636 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = first;
#line 14585 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                next = this->internalParseTokenizerSingleStringCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 639 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) += next;
#line 14612 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerDoubleStringCharacters()
{
    RuleResult result;
    auto retval = internalParseTokenizerDoubleStringCharacters(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerDoubleStringCharacters(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String first{};
    String next{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDoubleStringCharacters;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    first = this->internalParseTokenizerDoubleStringCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 644 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = first;
#line 14663 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                next = this->internalParseTokenizerDoubleStringCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 647 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) += next;
#line 14690 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerSingleStringCharacter()
{
    RuleResult result;
    auto retval = internalParseTokenizerSingleStringCharacter(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerSingleStringCharacter(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t ch{};
    RawAndCookedString escape{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerSingleStringCharacter;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing \'", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'\'')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \'", isRequiredForSuccess__);
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(ruleResult__.success())
        ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
    else
        ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                ch = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 655 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(u"", ch);
#line 14818 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            escape = this->internalParseTokenizerEscapeSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 657 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = escape.cooked;
#line 14854 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLineContinuation(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 659 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 14880 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerDoubleStringCharacter()
{
    RuleResult result;
    auto retval = internalParseTokenizerDoubleStringCharacter(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerDoubleStringCharacter(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t ch{};
    RawAndCookedString escape{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerDoubleStringCharacter;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing \"", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'\"')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \"", isRequiredForSuccess__);
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(ruleResult__.success())
        ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
    else
        ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                ch = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 667 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(u"", ch);
#line 15005 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            escape = this->internalParseTokenizerEscapeSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 669 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = escape.cooked;
#line 15041 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLineContinuation(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 671 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 15067 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerLineContinuation()
{
    RuleResult result;
    auto retval = internalParseTokenizerLineContinuation(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerLineContinuation(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t lineTerminator{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerLineContinuation;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'\\')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        lineTerminator = this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 674 "javascript_tasklets/parser/parser_imp.peg"
                                                                                        (returnValue__) = appendCodePoint(u"\\", lineTerminator);
#line 15132 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenizerEscapeSequence()
{
    RuleResult result;
    auto retval = internalParseTokenizerEscapeSequence(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenizerEscapeSequence(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString characterEscape{};
    RawStringAndChar unicodeEscape{};
    RawStringAndChar hexEscape{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerEscapeSequence;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing 0", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'0')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing 0", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'9')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing decimal digit", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 679 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(u"0", String(1, u'\0'));
#line 15214 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        characterEscape = this->internalParseTokenizerCharacterEscapeSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 681 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = characterEscape;
#line 15232 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        unicodeEscape = this->internalParseTokenizerUnicodeEscapeSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 683 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(unicodeEscape.raw, appendCodePoint(u"", unicodeEscape.ch));
#line 15258 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        hexEscape = this->internalParseTokenizerHexEscapeSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 685 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(hexEscape.raw, appendCodePoint(u"", hexEscape.ch));
#line 15284 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawStringAndChar Parser::parseTokenizerHexEscapeSequence()
{
    RuleResult result;
    auto retval = internalParseTokenizerHexEscapeSequence(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawStringAndChar Parser::internalParseTokenizerHexEscapeSequence(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawStringAndChar returnValue__{};
    char32_t digit1{};
    char32_t digit2{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerHexEscapeSequence;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing x", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'x')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing x", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 690 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawStringAndChar(u"x", 0);
#line 15341 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                digit1 = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 692 "javascript_tasklets/parser/parser_imp.peg"
      
        auto digitValue = character_properties::javascriptDigitValue(digit1);
        if(digitValue < 0 || digitValue >= 0x10)
            (predicateReturnValue__) = "missing hex digit";
        else
        {
            (returnValue__).ch = (returnValue__).ch * 0x10 + digitValue;
            (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), digit1);
        }
    
#line 15387 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                digit2 = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
            const char *predicateReturnValue__ = nullptr;
            {
#line 703 "javascript_tasklets/parser/parser_imp.peg"
      
        auto digitValue = character_properties::javascriptDigitValue(digit2);
        if(digitValue < 0 || digitValue >= 0x10)
            (predicateReturnValue__) = "missing hex digit";
        else
        {
            (returnValue__).ch = (returnValue__).ch * 0x10 + digitValue;
            (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), digit2);
        }
    
#line 15436 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            if(predicateReturnValue__ != nullptr)
                ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenizerCharacterEscapeSequence()
{
    RuleResult result;
    auto retval = internalParseTokenizerCharacterEscapeSequence(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenizerCharacterEscapeSequence(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString singleEscapeCharacter{};
    char32_t nonEscapeCharacter{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerCharacterEscapeSequence;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    singleEscapeCharacter = this->internalParseTokenizerSingleEscapeCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 717 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = singleEscapeCharacter;
#line 15479 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        nonEscapeCharacter = this->internalParseTokenizerNonEscapeCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 719 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(appendCodePoint(u"", nonEscapeCharacter));
#line 15497 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenizerSingleEscapeCharacter()
{
    RuleResult result;
    auto retval = internalParseTokenizerSingleEscapeCharacter(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenizerSingleEscapeCharacter(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    char32_t char1{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerSingleEscapeCharacter;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
    }
    else
    {
        bool matches = false;
        if(this->source.get()[startLocation__] == U'\"')
        {
            matches = true;
        }
        else if(this->source.get()[startLocation__] == U'\'')
        {
            matches = true;
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            matches = true;
        }
        if(matches)
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            char1 = this->source.get()[startLocation__];
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \", \', or \\", isRequiredForSuccess__);
        }
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 724 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(appendCodePoint(u"", char1));
#line 15570 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing b", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'b')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing b", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 726 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(u"b", u"\b");
#line 15597 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing f", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'f')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing f", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 728 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(u"f", u"\f");
#line 15632 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing n", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'n')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing n", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 730 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(u"n", u"\n");
#line 15667 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing r", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'r')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing r", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 732 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(u"r", u"\r");
#line 15702 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing t", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U't')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing t", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 734 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(u"t", u"\t");
#line 15737 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing v", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'v')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing v", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 736 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(u"v", u"\v");
#line 15772 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

char32_t Parser::parseTokenizerNonEscapeCharacter()
{
    RuleResult result;
    auto retval = internalParseTokenizerNonEscapeCharacter(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

char32_t Parser::internalParseTokenizerNonEscapeCharacter(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    char32_t returnValue__{};
    char32_t ch{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerNonEscapeCharacter;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(ruleResult__.success())
        ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
    else
        ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(this->source.get()[startLocation__] >= U'0' && this->source.get()[startLocation__] <= U'9')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'u')
            {
                matches = true;
            }
            else if(this->source.get()[startLocation__] == U'x')
            {
                matches = true;
            }
            if(matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing [0-9ux]", isRequiredForSuccess__);
            }
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerSingleEscapeCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                ch = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 744 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = ch;
#line 15903 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

BodyAndFlags Parser::parseTokenizerRegularExpressionLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenizerRegularExpressionLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

BodyAndFlags Parser::internalParseTokenizerRegularExpressionLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    BodyAndFlags returnValue__{};
    String body{};
    String flags{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRegularExpressionLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'/')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        body = this->internalParseTokenizerRegularExpressionBody(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing /", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'/')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        flags = this->internalParseTokenizerRegularExpressionFlags(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 752 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = BodyAndFlags(body, flags);
#line 15988 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerRegularExpressionBody()
{
    RuleResult result;
    auto retval = internalParseTokenizerRegularExpressionBody(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerRegularExpressionBody(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRegularExpressionBody;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing *", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'*')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing *", isRequiredForSuccess__);
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(ruleResult__.success())
        ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
    else
        ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 756 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 16042 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerRegularExpressionChar(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 759 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) += value;
#line 16061 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                value = this->internalParseTokenizerRegularExpressionChar(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 759 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) += value;
#line 16084 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerRegularExpressionChar()
{
    RuleResult result;
    auto retval = internalParseTokenizerRegularExpressionChar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerRegularExpressionChar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t nonTerminator{};
    String backslashSequence{};
    String classValue{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRegularExpressionChar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
    }
    else
    {
        bool matches = false;
        if(this->source.get()[startLocation__] == U'/')
        {
            matches = true;
        }
        else if(this->source.get()[startLocation__] == U'[')
        {
            matches = true;
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            matches = true;
        }
        if(matches)
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing /, [, or \\", isRequiredForSuccess__);
        }
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(ruleResult__.success())
        ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
    else
        ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                nonTerminator = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 766 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(u"", nonTerminator);
#line 16205 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        backslashSequence = this->internalParseTokenizerRegularExpressionBackslashSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 768 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = backslashSequence;
#line 16223 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        classValue = this->internalParseTokenizerRegularExpressionClass(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 770 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = classValue;
#line 16249 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerRegularExpressionBackslashSequence()
{
    RuleResult result;
    auto retval = internalParseTokenizerRegularExpressionBackslashSequence(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerRegularExpressionBackslashSequence(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t nonTerminator{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRegularExpressionBackslashSequence;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'\\')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                nonTerminator = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 777 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(u"\\", nonTerminator);
#line 16343 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerRegularExpressionClass()
{
    RuleResult result;
    auto retval = internalParseTokenizerRegularExpressionClass(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerRegularExpressionClass(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String classChar{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRegularExpressionClass;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing [", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'[')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing [", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 782 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"[";
#line 16391 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                classChar = this->internalParseTokenizerRegularExpressionClassChar(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 785 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) += classChar;
#line 16418 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing ]", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U']')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing ]", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 788 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) += u"]";
#line 16459 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerRegularExpressionClassChar()
{
    RuleResult result;
    auto retval = internalParseTokenizerRegularExpressionClassChar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerRegularExpressionClassChar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t nonTerminator{};
    String backslashSequence{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRegularExpressionClassChar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
    }
    else
    {
        bool matches = false;
        if(this->source.get()[startLocation__] == U'\\')
        {
            matches = true;
        }
        else if(this->source.get()[startLocation__] == U']')
        {
            matches = true;
        }
        if(matches)
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\ or ]", isRequiredForSuccess__);
        }
    }
    isRequiredForSuccess__ = !isRequiredForSuccess__;
    if(ruleResult__.success())
        ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
    else
        ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
        }
        else
        {
            bool matches = false;
            if(!matches)
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                nonTerminator = this->source.get()[startLocation__];
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "[] not allowed here", isRequiredForSuccess__);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 795 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = appendCodePoint(u"", nonTerminator);
#line 16564 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        backslashSequence = this->internalParseTokenizerRegularExpressionBackslashSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 797 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = backslashSequence;
#line 16582 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenizerRegularExpressionFlags()
{
    RuleResult result;
    auto retval = internalParseTokenizerRegularExpressionFlags(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenizerRegularExpressionFlags(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    char32_t ch{};
    RawStringAndChar unicodeEscape{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerRegularExpressionFlags;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
#line 801 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"";
#line 16623 "javascript_tasklets/parser/parser_imp.cpp"
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                ch = this->internalParseTokenizerEscapelessIdentifierPart(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 804 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = appendCodePoint(std::move((returnValue__)), ch);
#line 16648 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail())
                {
                    Parser::RuleResult lastRuleResult__ = ruleResult__;
                    if(startLocation__ >= this->sourceSize)
                    {
                        ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
                    }
                    else if(this->source.get()[startLocation__] == U'\\')
                    {
                        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                    }
                    else
                    {
                        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
                    }
                    if(ruleResult__.success())
                    {
                        auto savedStartLocation__ = startLocation__;
                        startLocation__ = ruleResult__.location;
                        ruleResult__ = Parser::RuleResult();
                        unicodeEscape = this->internalParseTokenizerUnicodeEscapeSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
                        assert(!ruleResult__.empty());
                        startLocation__ = savedStartLocation__;
                    }
                    if(ruleResult__.success())
                    {
                        auto savedStartLocation__ = startLocation__;
                        startLocation__ = ruleResult__.location;
                        {
#line 807 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) += u"\\" + unicodeEscape.raw;
#line 16684 "javascript_tasklets/parser/parser_imp.cpp"
                        }
                        ruleResult__ = this->makeSuccess(startLocation__);
                        startLocation__ = savedStartLocation__;
                    }
                    if(ruleResult__.success())
                    {
                        if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                        {
                            ruleResult__.endLocation = lastRuleResult__.endLocation;
                        }
                    }
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenizerNoSubstitutionTemplate()
{
    RuleResult result;
    auto retval = internalParseTokenizerNoSubstitutionTemplate(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenizerNoSubstitutionTemplate(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerNoSubstitutionTemplate;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing `", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'`')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing `", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                value = this->internalParseTokenizerTemplateCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 816 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__).raw += value.raw; (returnValue__).cooked += value.cooked;
#line 16790 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing `", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'`')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing `", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenizerTemplateHead()
{
    RuleResult result;
    auto retval = internalParseTokenizerTemplateHead(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenizerTemplateHead(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTemplateHead;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing `", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'`')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing `", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                value = this->internalParseTokenizerTemplateCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 824 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__).raw += value.raw; (returnValue__).cooked += value.cooked;
#line 16882 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing $", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'$')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing $", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            if(startLocation__ >= this->sourceSize)
            {
                ruleResult__ = this->makeFail(startLocation__, "missing {", isRequiredForSuccess__);
            }
            else if(this->source.get()[startLocation__] == U'{')
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing {", isRequiredForSuccess__);
            }
            startLocation__ = savedStartLocation__;
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenizerTemplateMiddle()
{
    RuleResult result;
    auto retval = internalParseTokenizerTemplateMiddle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenizerTemplateMiddle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTemplateMiddle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing }", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'}')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing }", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                value = this->internalParseTokenizerTemplateCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 832 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__).raw += value.raw; (returnValue__).cooked += value.cooked;
#line 16992 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing $", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'$')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing $", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            if(startLocation__ >= this->sourceSize)
            {
                ruleResult__ = this->makeFail(startLocation__, "missing {", isRequiredForSuccess__);
            }
            else if(this->source.get()[startLocation__] == U'{')
            {
                ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
            }
            else
            {
                ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing {", isRequiredForSuccess__);
            }
            startLocation__ = savedStartLocation__;
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenizerTemplateTail()
{
    RuleResult result;
    auto retval = internalParseTokenizerTemplateTail(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenizerTemplateTail(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTemplateTail;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing }", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'}')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing }", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                value = this->internalParseTokenizerTemplateCharacter(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 840 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__).raw += value.raw; (returnValue__).cooked += value.cooked;
#line 17102 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing `", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'`')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing `", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenizerTemplateCharacter()
{
    RuleResult result;
    auto retval = internalParseTokenizerTemplateCharacter(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenizerTemplateCharacter(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString escapeSequence{};
    char32_t lineContinuation{};
    char32_t lineTerminator{};
    char32_t ch{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenizerTemplateCharacter;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing $", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'$')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing $", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing {", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'{')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing {", isRequiredForSuccess__);
        }
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 847 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = RawAndCookedString(u"$");
#line 17206 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            escapeSequence = this->internalParseTokenizerEscapeSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 850 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__).raw += u"\\" + escapeSequence.raw; (returnValue__).cooked += escapeSequence.cooked;
#line 17242 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing \\", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U'\\')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing \\", isRequiredForSuccess__);
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            lineContinuation = this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 853 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw) + u"\\", lineContinuation);
#line 17286 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        lineTerminator = this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 855 "javascript_tasklets/parser/parser_imp.peg"
     
        (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), lineTerminator);
        (returnValue__).cooked = appendCodePoint(std::move((returnValue__).cooked), lineTerminator);
    
#line 17315 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLineTerminatorSequence(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            if(startLocation__ >= this->sourceSize)
            {
                ruleResult__ = this->makeFail(startLocation__, "unexpected end of input", isRequiredForSuccess__);
            }
            else
            {
                bool matches = false;
                if(this->source.get()[startLocation__] == U'$')
                {
                    matches = true;
                }
                else if(this->source.get()[startLocation__] == U'\\')
                {
                    matches = true;
                }
                else if(this->source.get()[startLocation__] == U'`')
                {
                    matches = true;
                }
                if(!matches)
                {
                    ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
                    ch = this->source.get()[startLocation__];
                }
                else
                {
                    ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "$, \\, or ` not allowed here", isRequiredForSuccess__);
                }
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 861 "javascript_tasklets/parser/parser_imp.peg"
     
        (returnValue__).raw = appendCodePoint(std::move((returnValue__).raw), ch);
        (returnValue__).cooked = appendCodePoint(std::move((returnValue__).cooked), ch);
    
#line 17385 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseIdentifierName()
{
    RuleResult result;
    auto retval = internalParseIdentifierName(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseIdentifierName(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultIdentifierName;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerIdentifierName(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 870 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = value;
#line 17441 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenNoSubstitutionTemplate()
{
    RuleResult result;
    auto retval = internalParseTokenNoSubstitutionTemplate(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenNoSubstitutionTemplate(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenNoSubstitutionTemplate;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerNoSubstitutionTemplate(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 876 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = std::move(value);
#line 17489 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenTemplateHead()
{
    RuleResult result;
    auto retval = internalParseTokenTemplateHead(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenTemplateHead(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenTemplateHead;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerTemplateHead(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 882 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = std::move(value);
#line 17537 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenTemplateMiddle()
{
    RuleResult result;
    auto retval = internalParseTokenTemplateMiddle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenTemplateMiddle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenTemplateMiddle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerTemplateMiddle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 888 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = std::move(value);
#line 17585 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

RawAndCookedString Parser::parseTokenTemplateTail()
{
    RuleResult result;
    auto retval = internalParseTokenTemplateTail(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

RawAndCookedString Parser::internalParseTokenTemplateTail(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    RawAndCookedString returnValue__{};
    RawAndCookedString value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenTemplateTail;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerTemplateTail(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 894 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = std::move(value);
#line 17633 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

void Parser::parseTokenAwait()
{
    RuleResult result;
    internalParseTokenAwait(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenAwait(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenAwait;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerAwait(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenBreak()
{
    RuleResult result;
    internalParseTokenBreak(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenBreak(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenBreak;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerBreak(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenCase()
{
    RuleResult result;
    internalParseTokenCase(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenCase(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenCase;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerCase(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenCatch()
{
    RuleResult result;
    internalParseTokenCatch(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenCatch(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenCatch;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerCatch(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenClass()
{
    RuleResult result;
    internalParseTokenClass(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenClass(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenClass;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerClass(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenConst()
{
    RuleResult result;
    internalParseTokenConst(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenConst(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenConst;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerConst(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenContinue()
{
    RuleResult result;
    internalParseTokenContinue(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenContinue(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenContinue;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerContinue(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenDebugger()
{
    RuleResult result;
    internalParseTokenDebugger(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenDebugger(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenDebugger;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerDebugger(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenDefault()
{
    RuleResult result;
    internalParseTokenDefault(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenDefault(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenDefault;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerDefault(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenDelete()
{
    RuleResult result;
    internalParseTokenDelete(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenDelete(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenDelete;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerDelete(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenDo()
{
    RuleResult result;
    internalParseTokenDo(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenDo(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenDo;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerDo(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenElse()
{
    RuleResult result;
    internalParseTokenElse(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenElse(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenElse;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerElse(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenEnum()
{
    RuleResult result;
    internalParseTokenEnum(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenEnum(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenEnum;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEnum(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenExport()
{
    RuleResult result;
    internalParseTokenExport(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenExport(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenExport;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerExport(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenExtends()
{
    RuleResult result;
    internalParseTokenExtends(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenExtends(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenExtends;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerExtends(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenFalse()
{
    RuleResult result;
    internalParseTokenFalse(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenFalse(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenFalse;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFalse(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenFinally()
{
    RuleResult result;
    internalParseTokenFinally(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenFinally(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenFinally;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFinally(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenFor()
{
    RuleResult result;
    internalParseTokenFor(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenFor(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenFor;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFor(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenFunction()
{
    RuleResult result;
    internalParseTokenFunction(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenFunction(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenFunction;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFunction(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenIf()
{
    RuleResult result;
    internalParseTokenIf(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenIf(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenIf;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerIf(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenImplements()
{
    RuleResult result;
    internalParseTokenImplements(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenImplements(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenImplements;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerImplements(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenImport()
{
    RuleResult result;
    internalParseTokenImport(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenImport(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenImport;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerImport(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenIn()
{
    RuleResult result;
    internalParseTokenIn(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenIn(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenIn;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerIn(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenInstanceOf()
{
    RuleResult result;
    internalParseTokenInstanceOf(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenInstanceOf(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenInstanceOf;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerInstanceOf(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenInterface()
{
    RuleResult result;
    internalParseTokenInterface(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenInterface(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenInterface;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerInterface(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenNew()
{
    RuleResult result;
    internalParseTokenNew(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenNew(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenNew;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerNew(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenNull()
{
    RuleResult result;
    internalParseTokenNull(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenNull(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenNull;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerNull(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPackage()
{
    RuleResult result;
    internalParseTokenPackage(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPackage(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPackage;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPackage(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPrivate()
{
    RuleResult result;
    internalParseTokenPrivate(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPrivate(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPrivate;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPrivate(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenProtected()
{
    RuleResult result;
    internalParseTokenProtected(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenProtected(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenProtected;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerProtected(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPublic()
{
    RuleResult result;
    internalParseTokenPublic(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPublic(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPublic;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPublic(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenReturn()
{
    RuleResult result;
    internalParseTokenReturn(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenReturn(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenReturn;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerReturn(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenSuper()
{
    RuleResult result;
    internalParseTokenSuper(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenSuper(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenSuper;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerSuper(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenSwitch()
{
    RuleResult result;
    internalParseTokenSwitch(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenSwitch(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenSwitch;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerSwitch(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenThis()
{
    RuleResult result;
    internalParseTokenThis(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenThis(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenThis;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerThis(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenThrow()
{
    RuleResult result;
    internalParseTokenThrow(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenThrow(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenThrow;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerThrow(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenTrue()
{
    RuleResult result;
    internalParseTokenTrue(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenTrue(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenTrue;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerTrue(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenTry()
{
    RuleResult result;
    internalParseTokenTry(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenTry(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenTry;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerTry(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenTypeOf()
{
    RuleResult result;
    internalParseTokenTypeOf(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenTypeOf(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenTypeOf;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerTypeOf(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenVar()
{
    RuleResult result;
    internalParseTokenVar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenVar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenVar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerVar(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenVoid()
{
    RuleResult result;
    internalParseTokenVoid(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenVoid(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenVoid;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerVoid(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenWhile()
{
    RuleResult result;
    internalParseTokenWhile(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenWhile(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenWhile;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerWhile(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenWith()
{
    RuleResult result;
    internalParseTokenWith(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenWith(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenWith;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerWith(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenYield()
{
    RuleResult result;
    internalParseTokenYield(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenYield(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenYield;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerYield(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenLBrace()
{
    RuleResult result;
    internalParseTokenLBrace(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenLBrace(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenLBrace;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLBrace(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRBrace()
{
    RuleResult result;
    internalParseTokenRBrace(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRBrace(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRBrace;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRBrace(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenLParen()
{
    RuleResult result;
    internalParseTokenLParen(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenLParen(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenLParen;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLParen(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRParen()
{
    RuleResult result;
    internalParseTokenRParen(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRParen(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRParen;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRParen(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenLBracket()
{
    RuleResult result;
    internalParseTokenLBracket(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenLBracket(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenLBracket;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLBracket(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRBracket()
{
    RuleResult result;
    internalParseTokenRBracket(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRBracket(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRBracket;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRBracket(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPeriod()
{
    RuleResult result;
    internalParseTokenPeriod(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPeriod(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPeriod;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPeriod(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenEllipsis()
{
    RuleResult result;
    internalParseTokenEllipsis(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenEllipsis(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenEllipsis;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEllipsis(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenSemicolon()
{
    RuleResult result;
    internalParseTokenSemicolon(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenSemicolon(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenSemicolon;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerSemicolon(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenComma()
{
    RuleResult result;
    internalParseTokenComma(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenComma(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenComma;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerComma(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenLAngle()
{
    RuleResult result;
    internalParseTokenLAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenLAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenLAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRAngle()
{
    RuleResult result;
    internalParseTokenRAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenLAngleEqual()
{
    RuleResult result;
    internalParseTokenLAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenLAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenLAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRAngleEqual()
{
    RuleResult result;
    internalParseTokenRAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenEqualEqual()
{
    RuleResult result;
    internalParseTokenEqualEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenEqualEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenEqualEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEqualEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenEMarkEqual()
{
    RuleResult result;
    internalParseTokenEMarkEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenEMarkEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenEMarkEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEMarkEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenEqualEqualEqual()
{
    RuleResult result;
    internalParseTokenEqualEqualEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenEqualEqualEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenEqualEqualEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEqualEqualEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenEMarkEqualEqual()
{
    RuleResult result;
    internalParseTokenEMarkEqualEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenEMarkEqualEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenEMarkEqualEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEMarkEqualEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPlus()
{
    RuleResult result;
    internalParseTokenPlus(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPlus(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPlus;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPlus(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenMinus()
{
    RuleResult result;
    internalParseTokenMinus(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenMinus(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenMinus;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerMinus(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenStar()
{
    RuleResult result;
    internalParseTokenStar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenStar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenStar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerStar(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenStarStar()
{
    RuleResult result;
    internalParseTokenStarStar(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenStarStar(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenStarStar;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerStarStar(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPercent()
{
    RuleResult result;
    internalParseTokenPercent(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPercent(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPercent;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPercent(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPlusPlus()
{
    RuleResult result;
    internalParseTokenPlusPlus(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPlusPlus(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPlusPlus;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPlusPlus(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenMinusMinus()
{
    RuleResult result;
    internalParseTokenMinusMinus(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenMinusMinus(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenMinusMinus;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerMinusMinus(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenLAngleLAngle()
{
    RuleResult result;
    internalParseTokenLAngleLAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenLAngleLAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenLAngleLAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLAngleLAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRAngleRAngle()
{
    RuleResult result;
    internalParseTokenRAngleRAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRAngleRAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRAngleRAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleRAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRAngleRAngleRAngle()
{
    RuleResult result;
    internalParseTokenRAngleRAngleRAngle(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRAngleRAngleRAngle(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRAngleRAngleRAngle;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleRAngleRAngle(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenAmp()
{
    RuleResult result;
    internalParseTokenAmp(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenAmp(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenAmp;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerAmp(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPipe()
{
    RuleResult result;
    internalParseTokenPipe(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPipe(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPipe;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPipe(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenCaret()
{
    RuleResult result;
    internalParseTokenCaret(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenCaret(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenCaret;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerCaret(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenEMark()
{
    RuleResult result;
    internalParseTokenEMark(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenEMark(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenEMark;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEMark(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenTilde()
{
    RuleResult result;
    internalParseTokenTilde(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenTilde(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenTilde;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerTilde(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenAmpAmp()
{
    RuleResult result;
    internalParseTokenAmpAmp(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenAmpAmp(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenAmpAmp;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerAmpAmp(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPipePipe()
{
    RuleResult result;
    internalParseTokenPipePipe(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPipePipe(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPipePipe;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPipePipe(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenQMark()
{
    RuleResult result;
    internalParseTokenQMark(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenQMark(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenQMark;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerQMark(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenColon()
{
    RuleResult result;
    internalParseTokenColon(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenColon(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenColon;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerColon(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenEqual()
{
    RuleResult result;
    internalParseTokenEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPlusEqual()
{
    RuleResult result;
    internalParseTokenPlusEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPlusEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPlusEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPlusEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenMinusEqual()
{
    RuleResult result;
    internalParseTokenMinusEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenMinusEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenMinusEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerMinusEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenStarEqual()
{
    RuleResult result;
    internalParseTokenStarEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenStarEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenStarEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerStarEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPercentEqual()
{
    RuleResult result;
    internalParseTokenPercentEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPercentEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPercentEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPercentEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenLAngleLAngleEqual()
{
    RuleResult result;
    internalParseTokenLAngleLAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenLAngleLAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenLAngleLAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerLAngleLAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRAngleRAngleEqual()
{
    RuleResult result;
    internalParseTokenRAngleRAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRAngleRAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRAngleRAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleRAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenRAngleRAngleRAngleEqual()
{
    RuleResult result;
    internalParseTokenRAngleRAngleRAngleEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenRAngleRAngleRAngleEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRAngleRAngleRAngleEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerRAngleRAngleRAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenAmpEqual()
{
    RuleResult result;
    internalParseTokenAmpEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenAmpEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenAmpEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerAmpEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenPipeEqual()
{
    RuleResult result;
    internalParseTokenPipeEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenPipeEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenPipeEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerPipeEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenCaretEqual()
{
    RuleResult result;
    internalParseTokenCaretEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenCaretEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenCaretEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerCaretEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenArrow()
{
    RuleResult result;
    internalParseTokenArrow(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenArrow(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenArrow;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerArrow(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenFSlash()
{
    RuleResult result;
    internalParseTokenFSlash(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenFSlash(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenFSlash;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFSlash(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

void Parser::parseTokenFSlashEqual()
{
    RuleResult result;
    internalParseTokenFSlashEqual(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
}

void Parser::internalParseTokenFSlashEqual(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenFSlashEqual;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerFSlashEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
}

template <bool isModule, bool isStrict>
String Parser::parseTokenIdentifier()
{
    RuleResult result;
    auto retval = internalParseTokenIdentifier<isModule, isStrict>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict>
String Parser::internalParseTokenIdentifier(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenIdentifier[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerReservedWord<isModule, isStrict>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerIdentifierName(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 998 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = value;
#line 20738 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
String Parser::parseTokenIdentifierOrYield()
{
    RuleResult result;
    auto retval = internalParseTokenIdentifierOrYield<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
String Parser::internalParseTokenIdentifierOrYield(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenIdentifierOrYield[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1001 "javascript_tasklets/parser/parser_imp.peg"
      
        if(!canHaveYieldOperator || isStrict)
            (predicateReturnValue__) = "yield not allowed here";
    
#line 20777 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenYield(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1006 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = u"yield";
#line 20799 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenIdentifier<isModule, isStrict>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1008 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = value;
#line 20817 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenNumericLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenNumericLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenNumericLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenNumericLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerNumericLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1014 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = value;
#line 20873 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

String Parser::parseTokenStringLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenStringLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

String Parser::internalParseTokenStringLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    String returnValue__{};
    String value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenStringLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerStringLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1020 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = value;
#line 20921 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

BodyAndFlags Parser::parseTokenRegularExpressionLiteral()
{
    RuleResult result;
    auto retval = internalParseTokenRegularExpressionLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

BodyAndFlags Parser::internalParseTokenRegularExpressionLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    BodyAndFlags returnValue__{};
    BodyAndFlags value{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTokenRegularExpressionLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenizerTokenSeperator<true>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        value = this->internalParseTokenizerRegularExpressionLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1026 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = value;
#line 20969 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

ExpressionPointer Parser::parseRegularExpressionLiteral()
{
    RuleResult result;
    auto retval = internalParseRegularExpressionLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

ExpressionPointer Parser::internalParseRegularExpressionLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    BodyAndFlags regExp{};
    auto &ruleResult__ = this->getResults(startLocation__).resultRegularExpressionLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    regExp = this->internalParseTokenRegularExpressionLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1031 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionRegExpLiteral>((static_cast<const ::std::size_t &>(startLocation__)), regExp.body, regExp.flags);
#line 21008 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::parseIdentifierReference()
{
    RuleResult result;
    auto retval = internalParseIdentifierReference<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseIdentifierReference(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    String name{};
    auto &ruleResult__ = this->getResults(startLocation__).resultIdentifierReference[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    name = this->internalParseTokenIdentifierOrYield<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1036 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionIdentifierReference>((static_cast<const ::std::size_t &>(startLocation__)), name);
#line 21049 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict>
ExpressionPointer Parser::parseFunctionExpression()
{
    RuleResult result;
    auto retval = internalParseFunctionExpression<isModule, isStrict>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict>
ExpressionPointer Parser::internalParseFunctionExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultFunctionExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1040 "javascript_tasklets/parser/parser_imp.peg"
      
        (predicateReturnValue__) = "FunctionExpression not implemented";
#warning implement function expression
    
#line 21087 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict>
ExpressionPointer Parser::parseGeneratorExpression()
{
    RuleResult result;
    auto retval = internalParseGeneratorExpression<isModule, isStrict>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict>
ExpressionPointer Parser::internalParseGeneratorExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultGeneratorExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1046 "javascript_tasklets/parser/parser_imp.peg"
      
        (predicateReturnValue__) = "GeneratorExpression not implemented";
#warning implement generator expression
    
#line 21126 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::parseClassExpression()
{
    RuleResult result;
    auto retval = internalParseClassExpression<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseClassExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultClassExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1052 "javascript_tasklets/parser/parser_imp.peg"
      
        (predicateReturnValue__) = "ClassExpression not implemented";
#warning implement class expression
    
#line 21165 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::parseParenthesizedExpression()
{
    RuleResult result;
    auto retval = internalParseParenthesizedExpression<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseParenthesizedExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    ExpressionPointer expression{};
    auto &ruleResult__ = this->getResults(startLocation__).resultParenthesizedExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    if(startLocation__ >= this->sourceSize)
    {
        ruleResult__ = this->makeFail(startLocation__, "missing (", isRequiredForSuccess__);
    }
    else if(this->source.get()[startLocation__] == U'(')
    {
        ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
    }
    else
    {
        ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing (", isRequiredForSuccess__);
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        expression = this->internalParseExpression<isModule, isStrict, true, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        if(startLocation__ >= this->sourceSize)
        {
            ruleResult__ = this->makeFail(startLocation__, "missing )", isRequiredForSuccess__);
        }
        else if(this->source.get()[startLocation__] == U')')
        {
            ruleResult__ = this->makeSuccess(startLocation__ + 1, startLocation__ + 1);
        }
        else
        {
            ruleResult__ = this->makeFail(startLocation__, startLocation__ + 1, "missing )", isRequiredForSuccess__);
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenizerTokenSeperatorWithLineTerminator(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
            ruleResult__.location = startLocation__;
        if(ruleResult__.fail())
        {
            Parser::RuleResult lastRuleResult__ = ruleResult__;
            isRequiredForSuccess__ = !isRequiredForSuccess__;
            ruleResult__ = Parser::RuleResult();
            this->internalParseTokenArrow(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            isRequiredForSuccess__ = !isRequiredForSuccess__;
            if(ruleResult__.success())
                ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
            else
                ruleResult__ = this->makeSuccess(startLocation__);
            if(ruleResult__.success())
            {
                if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                {
                    ruleResult__.endLocation = lastRuleResult__.endLocation;
                }
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1065 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = expression;
#line 21274 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::parsePrimaryExpression()
{
    RuleResult result;
    auto retval = internalParsePrimaryExpression<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParsePrimaryExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    ExpressionPointer identifierReference{};
    ExpressionPointer literal{};
    ExpressionPointer arrayLiteral{};
    ExpressionPointer objectLiteral{};
    ExpressionPointer functionExpression{};
    ExpressionPointer classExpression{};
    ExpressionPointer generatorExpression{};
    ExpressionPointer regularExpressionLiteral{};
    ExpressionPointer templateLiteral{};
    ExpressionPointer parenthesizedExpression{};
    auto &ruleResult__ = this->getResults(startLocation__).resultPrimaryExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenThis(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1069 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionThis>((static_cast<const ::std::size_t &>(startLocation__)));
#line 21324 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        identifierReference = this->internalParseIdentifierReference<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1071 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = identifierReference;
#line 21342 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        literal = this->internalParseLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1073 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = literal;
#line 21368 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        arrayLiteral = this->internalParseArrayLiteral<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1075 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arrayLiteral;
#line 21394 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        objectLiteral = this->internalParseObjectLiteral<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1077 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = objectLiteral;
#line 21420 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        functionExpression = this->internalParseFunctionExpression<isModule, isStrict>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1079 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = functionExpression;
#line 21446 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        classExpression = this->internalParseClassExpression<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1081 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = classExpression;
#line 21472 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        generatorExpression = this->internalParseGeneratorExpression<isModule, isStrict>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1083 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = generatorExpression;
#line 21498 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        regularExpressionLiteral = this->internalParseRegularExpressionLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1085 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = regularExpressionLiteral;
#line 21524 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        templateLiteral = this->internalParseTemplateLiteral<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1087 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = templateLiteral;
#line 21550 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        parenthesizedExpression = this->internalParseParenthesizedExpression<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1089 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = parenthesizedExpression;
#line 21576 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

ExpressionPointer Parser::parseLiteral()
{
    RuleResult result;
    auto retval = internalParseLiteral(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

ExpressionPointer Parser::internalParseLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    String numericLiteral{};
    String stringLiteral{};
    auto &ruleResult__ = this->getResults(startLocation__).resultLiteral;
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenNull(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1094 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionNullLiteral>((static_cast<const ::std::size_t &>(startLocation__)));
#line 21624 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenFalse(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1096 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionBooleanLiteral>((static_cast<const ::std::size_t &>(startLocation__)), false);
#line 21642 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenTrue(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1098 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionBooleanLiteral>((static_cast<const ::std::size_t &>(startLocation__)), true);
#line 21668 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        numericLiteral = this->internalParseTokenNumericLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1100 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionNumericLiteral>((static_cast<const ::std::size_t &>(startLocation__)), value::StringHandle::toNumberValue(numericLiteral));
#line 21694 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        stringLiteral = this->internalParseTokenStringLiteral(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1102 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionStringLiteral>((static_cast<const ::std::size_t &>(startLocation__)), stringLiteral);
#line 21720 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ArrayLiteralElementPointer Parser::parseArrayLiteralElement()
{
    RuleResult result;
    auto retval = internalParseArrayLiteralElement<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ArrayLiteralElementPointer returnValue__{};
    ExpressionPointer spreadExpression{};
    ExpressionPointer expression{};
    auto &ruleResult__ = this->getResults(startLocation__).resultArrayLiteralElement[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenComma(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenRBracket(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
        ruleResult__.location = startLocation__;
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1110 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = nullptr;
#line 21787 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenEllipsis(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            spreadExpression = this->internalParseAssignmentExpression<isModule, isStrict, true, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1113 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::SpreadArrayLiteralElement>((static_cast<const ::std::size_t &>(startLocation__)), spreadExpression);
#line 21814 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        expression = this->internalParseAssignmentExpression<isModule, isStrict, true, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1115 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionArrayLiteralElement>((static_cast<const ::std::size_t &>(startLocation__)), expression);
#line 21840 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ArrayLiteralElementList Parser::parseArrayLiteralElementList()
{
    RuleResult result;
    auto retval = internalParseArrayLiteralElementList<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ArrayLiteralElementList Parser::internalParseArrayLiteralElementList(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ArrayLiteralElementList returnValue__{};
    ArrayLiteralElementPointer element{};
    ArrayLiteralElementPointer lastElement{};
    auto &ruleResult__ = this->getResults(startLocation__).resultArrayLiteralElementList[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    {
        auto savedStartLocation__ = startLocation__;
        auto &savedRuleResult__ = ruleResult__;
        while(true)
        {
            Parser::RuleResult ruleResult__;
            startLocation__ = savedRuleResult__.location;
            ruleResult__ = Parser::RuleResult();
            element = this->internalParseArrayLiteralElement<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenComma(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
#line 1122 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__).push_back(element);
#line 21907 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.fail() || ruleResult__.location == startLocation__)
            {
                savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                startLocation__ = savedStartLocation__;
                break;
            }
            savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
        }
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        lastElement = this->internalParseArrayLiteralElement<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1126 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__).push_back(lastElement);
#line 21935 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.fail())
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::parseArrayLiteral()
{
    RuleResult result;
    auto retval = internalParseArrayLiteral<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseArrayLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    ArrayLiteralElementList elements{};
    auto &ruleResult__ = this->getResults(startLocation__).resultArrayLiteral[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    this->internalParseTokenLBracket(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        elements = this->internalParseArrayLiteralElementList<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenRBracket(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1134 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arena.make<ast::ExpressionArrayLiteral>((static_cast<const ::std::size_t &>(startLocation__)), std::move(elements));
#line 21998 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::parseObjectLiteral()
{
    RuleResult result;
    auto retval = internalParseObjectLiteral<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseObjectLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultObjectLiteral[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1138 "javascript_tasklets/parser/parser_imp.peg"
      
        (predicateReturnValue__) = "ObjectLiteral not implemented";
#warning implement object literal
    
#line 22036 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ast::TemplateRest Parser::parseTemplateLiteralRest()
{
    RuleResult result;
    auto retval = internalParseTemplateLiteralRest<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ast::TemplateRest Parser::internalParseTemplateLiteralRest(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ast::TemplateRest returnValue__{};
    ExpressionPointer expression{};
    RawAndCookedString middle{};
    ExpressionPointer lastExpression{};
    RawAndCookedString tail{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTemplateLiteralRest[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = this->makeSuccess(startLocation__);
    {
        auto savedStartLocation__ = startLocation__;
        auto &savedRuleResult__ = ruleResult__;
        while(true)
        {
            Parser::RuleResult ruleResult__;
            startLocation__ = savedRuleResult__.location;
            ruleResult__ = Parser::RuleResult();
            expression = this->internalParseExpression<isModule, isStrict, true, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                ruleResult__ = Parser::RuleResult();
                middle = this->internalParseTokenTemplateMiddle(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
#line 1147 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__).emplace_back(expression, ast::TemplateString(middle.raw, middle.cooked));
#line 22098 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.fail() || ruleResult__.location == startLocation__)
            {
                savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                startLocation__ = savedStartLocation__;
                break;
            }
            savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
        }
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        lastExpression = this->internalParseExpression<isModule, isStrict, true, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = Parser::RuleResult();
        tail = this->internalParseTokenTemplateTail(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1151 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__).emplace_back(lastExpression, ast::TemplateString(tail.raw, tail.cooked));
#line 22137 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::parseTemplateLiteral()
{
    RuleResult result;
    auto retval = internalParseTemplateLiteral<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseTemplateLiteral(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    RawAndCookedString noSubstitutionTemplate{};
    RawAndCookedString first{};
    ast::TemplateRest rest{};
    auto &ruleResult__ = this->getResults(startLocation__).resultTemplateLiteral[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    noSubstitutionTemplate = this->internalParseTokenNoSubstitutionTemplate(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1156 "javascript_tasklets/parser/parser_imp.peg"
     
        (returnValue__) = arena.make<ast::ExpressionTemplateLiteral>((static_cast<const ::std::size_t &>(startLocation__)),
                                                        ast::TemplateString(noSubstitutionTemplate.raw,
                                                                       noSubstitutionTemplate.cooked));
    
#line 22184 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        first = this->internalParseTokenTemplateHead(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            rest = this->internalParseTemplateLiteralRest<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1163 "javascript_tasklets/parser/parser_imp.peg"
     
        (returnValue__) = arena.make<ast::ExpressionTemplateLiteral>((static_cast<const ::std::size_t &>(startLocation__)), ast::TemplateString(first.raw, first.cooked), std::move(rest));
    
#line 22213 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
ExpressionPointer Parser::parseArrowFunction()
{
    RuleResult result;
    auto retval = internalParseArrowFunction<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseArrowFunction(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultArrowFunction[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveInOperator)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1169 "javascript_tasklets/parser/parser_imp.peg"
      
        (predicateReturnValue__) = "ArrowFunction not implemented";
#warning implement arrow function
    
#line 22259 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveInOperator>
ExpressionPointer Parser::parseYieldExpression()
{
    RuleResult result;
    auto retval = internalParseYieldExpression<isModule, isStrict, canHaveInOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveInOperator>
ExpressionPointer Parser::internalParseYieldExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultYieldExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveInOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1175 "javascript_tasklets/parser/parser_imp.peg"
      
        (predicateReturnValue__) = "YieldExpression not implemented";
#warning implement yield expression
    
#line 22298 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::parseLeftHandSideExpression()
{
    RuleResult result;
    auto retval = internalParseLeftHandSideExpression<isModule, isStrict, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseLeftHandSideExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultLeftHandSideExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1181 "javascript_tasklets/parser/parser_imp.peg"
      
        (predicateReturnValue__) = "LeftHandSideExpression not implemented";
#warning implement left-hand-side expression
    
#line 22337 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
ExpressionPointer Parser::parseConditionalExpression()
{
    RuleResult result;
    auto retval = internalParseConditionalExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseConditionalExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    auto &ruleResult__ = this->getResults(startLocation__).resultConditionalExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveInOperator)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    {
        const char *predicateReturnValue__ = nullptr;
        {
#line 1187 "javascript_tasklets/parser/parser_imp.peg"
      
        (predicateReturnValue__) = "ConditionalExpression not implemented";
#warning implement conditional expression
    
#line 22376 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        if(predicateReturnValue__ != nullptr)
            ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
ExpressionPointer Parser::parseAssignmentExpression()
{
    RuleResult result;
    auto retval = internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseAssignmentExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    ExpressionPointer arrowFunction{};
    ExpressionPointer yieldExpression{};
    ExpressionPointer leftHandSideExpression{};
    ExpressionPointer assignmentExpression{};
    ExpressionPointer multiplyAssignmentExpression{};
    ExpressionPointer divideAssignmentExpression{};
    ExpressionPointer modulusAssignmentExpression{};
    ExpressionPointer addAssignmentExpression{};
    ExpressionPointer subtractAssignmentExpression{};
    ExpressionPointer lShiftAssignmentExpression{};
    ExpressionPointer arShiftAssignmentExpression{};
    ExpressionPointer urShiftAssignmentExpression{};
    ExpressionPointer andAssignmentExpression{};
    ExpressionPointer xorAssignmentExpression{};
    ExpressionPointer orAssignmentExpression{};
    ExpressionPointer conditionalExpression{};
    auto &ruleResult__ = this->getResults(startLocation__).resultAssignmentExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveInOperator)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    arrowFunction = this->internalParseArrowFunction<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1194 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = arrowFunction;
#line 22433 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenYield(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
                const char *predicateReturnValue__ = nullptr;
                {
#line 1197 "javascript_tasklets/parser/parser_imp.peg"
          
            if(!canHaveYieldOperator)
                (predicateReturnValue__) = "yield not allowed here";
        
#line 22456 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                if(predicateReturnValue__ != nullptr)
                    ruleResult__ = this->makeFail(startLocation__, predicateReturnValue__, isRequiredForSuccess__);
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
            ruleResult__.location = startLocation__;
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            yieldExpression = this->internalParseYieldExpression<isModule, isStrict, canHaveInOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1203 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = yieldExpression;
#line 22482 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        leftHandSideExpression = this->internalParseLeftHandSideExpression<isModule, isStrict, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            ruleResult__ = Parser::RuleResult();
            this->internalParseTokenEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
            assert(!ruleResult__.empty());
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                ruleResult__ = Parser::RuleResult();
                assignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.success())
            {
                auto savedStartLocation__ = startLocation__;
                startLocation__ = ruleResult__.location;
                {
#line 1208 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::AssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, assignmentExpression);
#line 22524 "javascript_tasklets/parser/parser_imp.cpp"
                }
                ruleResult__ = this->makeSuccess(startLocation__);
                startLocation__ = savedStartLocation__;
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenStarEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    multiplyAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1211 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::MultiplyAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, multiplyAssignmentExpression);
#line 22551 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenFSlashEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    divideAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1214 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::DivideAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, divideAssignmentExpression);
#line 22586 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenPercentEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    modulusAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1217 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::ModulusAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, modulusAssignmentExpression);
#line 22621 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenPlusEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    addAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1220 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::AddAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, addAssignmentExpression);
#line 22656 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenMinusEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    subtractAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1223 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::SubtractAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, subtractAssignmentExpression);
#line 22691 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenLAngleLAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    lShiftAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1226 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::LShiftAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, lShiftAssignmentExpression);
#line 22726 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenRAngleRAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    arShiftAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1229 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::ARShiftAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, arShiftAssignmentExpression);
#line 22761 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenRAngleRAngleRAngleEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    urShiftAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1232 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::URShiftAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, urShiftAssignmentExpression);
#line 22796 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenAmpEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    andAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1235 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::AndAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, andAssignmentExpression);
#line 22831 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenCaretEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    xorAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1238 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::XorAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, xorAssignmentExpression);
#line 22866 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            if(ruleResult__.fail())
            {
                Parser::RuleResult lastRuleResult__ = ruleResult__;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenPipeEqual(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    orAssignmentExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1241 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::OrAssignmentExpression>((static_cast<const ::std::size_t &>(startLocation__)), leftHandSideExpression, orAssignmentExpression);
#line 22901 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
                    {
                        ruleResult__.endLocation = lastRuleResult__.endLocation;
                    }
                }
            }
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    if(ruleResult__.fail())
    {
        Parser::RuleResult lastRuleResult__ = ruleResult__;
        ruleResult__ = Parser::RuleResult();
        conditionalExpression = this->internalParseConditionalExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        if(ruleResult__.success())
        {
            auto savedStartLocation__ = startLocation__;
            startLocation__ = ruleResult__.location;
            {
#line 1244 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = conditionalExpression;
#line 22937 "javascript_tasklets/parser/parser_imp.cpp"
            }
            ruleResult__ = this->makeSuccess(startLocation__);
            startLocation__ = savedStartLocation__;
        }
        if(ruleResult__.success())
        {
            if(lastRuleResult__.endLocation >= ruleResult__.endLocation)
            {
                ruleResult__.endLocation = lastRuleResult__.endLocation;
            }
        }
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
ExpressionPointer Parser::parseExpression()
{
    RuleResult result;
    auto retval = internalParseExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(0, result, true);
    assert(!result.empty());
    if(result.fail())
        throw ParseError(errorLocation, errorMessage);
    return retval;
}

template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
ExpressionPointer Parser::internalParseExpression(std::size_t startLocation__, RuleResult &ruleResultOut__, bool isRequiredForSuccess__)
{
    ExpressionPointer returnValue__{};
    ExpressionPointer firstExpression{};
    ExpressionPointer nextExpression{};
    auto &ruleResult__ = this->getResults(startLocation__).resultExpression[static_cast<std::size_t>(isModule)][static_cast<std::size_t>(isStrict)][static_cast<std::size_t>(canHaveInOperator)][static_cast<std::size_t>(canHaveYieldOperator)];
    if(!ruleResult__.empty() && (ruleResult__.fail() || !isRequiredForSuccess__))
    {
        ruleResultOut__ = ruleResult__;
        return returnValue__;
    }
    ruleResult__ = Parser::RuleResult();
    firstExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
    assert(!ruleResult__.empty());
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        {
#line 1248 "javascript_tasklets/parser/parser_imp.peg"
     (returnValue__) = firstExpression;
#line 22987 "javascript_tasklets/parser/parser_imp.cpp"
        }
        ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        ruleResult__ = this->makeSuccess(startLocation__);
        {
            auto savedStartLocation__ = startLocation__;
            auto &savedRuleResult__ = ruleResult__;
            while(true)
            {
                Parser::RuleResult ruleResult__;
                startLocation__ = savedRuleResult__.location;
                ruleResult__ = Parser::RuleResult();
                this->internalParseTokenComma(startLocation__, ruleResult__, isRequiredForSuccess__);
                assert(!ruleResult__.empty());
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    ruleResult__ = Parser::RuleResult();
                    nextExpression = this->internalParseAssignmentExpression<isModule, isStrict, canHaveInOperator, canHaveYieldOperator>(startLocation__, ruleResult__, isRequiredForSuccess__);
                    assert(!ruleResult__.empty());
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.success())
                {
                    auto savedStartLocation__ = startLocation__;
                    startLocation__ = ruleResult__.location;
                    {
#line 1252 "javascript_tasklets/parser/parser_imp.peg"
         (returnValue__) = arena.make<ast::ExpressionCommaOperator>((static_cast<const ::std::size_t &>(startLocation__)), (returnValue__), nextExpression);
#line 23023 "javascript_tasklets/parser/parser_imp.cpp"
                    }
                    ruleResult__ = this->makeSuccess(startLocation__);
                    startLocation__ = savedStartLocation__;
                }
                if(ruleResult__.fail() || ruleResult__.location == startLocation__)
                {
                    savedRuleResult__ = this->makeSuccess(savedRuleResult__.location, ruleResult__.endLocation);
                    startLocation__ = savedStartLocation__;
                    break;
                }
                savedRuleResult__ = this->makeSuccess(ruleResult__.location, ruleResult__.endLocation);
            }
        }
        startLocation__ = savedStartLocation__;
    }
    if(ruleResult__.success())
    {
        auto savedStartLocation__ = startLocation__;
        startLocation__ = ruleResult__.location;
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        ruleResult__ = Parser::RuleResult();
        this->internalParseTokenComma(startLocation__, ruleResult__, isRequiredForSuccess__);
        assert(!ruleResult__.empty());
        isRequiredForSuccess__ = !isRequiredForSuccess__;
        if(ruleResult__.success())
            ruleResult__ = this->makeFail(startLocation__, "not allowed here", isRequiredForSuccess__);
        else
            ruleResult__ = this->makeSuccess(startLocation__);
        startLocation__ = savedStartLocation__;
    }
    ruleResultOut__ = ruleResult__;
    return returnValue__;
}

template void Parser::parseTokenizerComment<false>();
template void Parser::internalParseTokenizerComment<false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerComment<true>();
template void Parser::internalParseTokenizerComment<true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template bool Parser::parseTokenizerMultiLineComment<false>();
template bool Parser::internalParseTokenizerMultiLineComment<false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template bool Parser::parseTokenizerMultiLineComment<true>();
template bool Parser::internalParseTokenizerMultiLineComment<true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerTokenSeperator<false>();
template void Parser::internalParseTokenizerTokenSeperator<false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerTokenSeperator<true>();
template void Parser::internalParseTokenizerTokenSeperator<true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerReservedWord<false, false>();
template void Parser::internalParseTokenizerReservedWord<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerReservedWord<false, true>();
template void Parser::internalParseTokenizerReservedWord<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerReservedWord<true, false>();
template void Parser::internalParseTokenizerReservedWord<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerReservedWord<true, true>();
template void Parser::internalParseTokenizerReservedWord<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerFutureReservedWord<false, false>();
template void Parser::internalParseTokenizerFutureReservedWord<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerFutureReservedWord<false, true>();
template void Parser::internalParseTokenizerFutureReservedWord<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerFutureReservedWord<true, false>();
template void Parser::internalParseTokenizerFutureReservedWord<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template void Parser::parseTokenizerFutureReservedWord<true, true>();
template void Parser::internalParseTokenizerFutureReservedWord<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifier<false, false>();
template String Parser::internalParseTokenIdentifier<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifier<false, true>();
template String Parser::internalParseTokenIdentifier<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifier<true, false>();
template String Parser::internalParseTokenIdentifier<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifier<true, true>();
template String Parser::internalParseTokenIdentifier<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifierOrYield<false, false, false>();
template String Parser::internalParseTokenIdentifierOrYield<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifierOrYield<false, false, true>();
template String Parser::internalParseTokenIdentifierOrYield<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifierOrYield<false, true, false>();
template String Parser::internalParseTokenIdentifierOrYield<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifierOrYield<false, true, true>();
template String Parser::internalParseTokenIdentifierOrYield<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifierOrYield<true, false, false>();
template String Parser::internalParseTokenIdentifierOrYield<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifierOrYield<true, false, true>();
template String Parser::internalParseTokenIdentifierOrYield<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifierOrYield<true, true, false>();
template String Parser::internalParseTokenIdentifierOrYield<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template String Parser::parseTokenIdentifierOrYield<true, true, true>();
template String Parser::internalParseTokenIdentifierOrYield<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseIdentifierReference<false, false, false>();
template ExpressionPointer Parser::internalParseIdentifierReference<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseIdentifierReference<false, false, true>();
template ExpressionPointer Parser::internalParseIdentifierReference<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseIdentifierReference<false, true, false>();
template ExpressionPointer Parser::internalParseIdentifierReference<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseIdentifierReference<false, true, true>();
template ExpressionPointer Parser::internalParseIdentifierReference<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseIdentifierReference<true, false, false>();
template ExpressionPointer Parser::internalParseIdentifierReference<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseIdentifierReference<true, false, true>();
template ExpressionPointer Parser::internalParseIdentifierReference<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseIdentifierReference<true, true, false>();
template ExpressionPointer Parser::internalParseIdentifierReference<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseIdentifierReference<true, true, true>();
template ExpressionPointer Parser::internalParseIdentifierReference<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseFunctionExpression<false, false>();
template ExpressionPointer Parser::internalParseFunctionExpression<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseFunctionExpression<false, true>();
template ExpressionPointer Parser::internalParseFunctionExpression<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseFunctionExpression<true, false>();
template ExpressionPointer Parser::internalParseFunctionExpression<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseFunctionExpression<true, true>();
template ExpressionPointer Parser::internalParseFunctionExpression<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseGeneratorExpression<false, false>();
template ExpressionPointer Parser::internalParseGeneratorExpression<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseGeneratorExpression<false, true>();
template ExpressionPointer Parser::internalParseGeneratorExpression<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseGeneratorExpression<true, false>();
template ExpressionPointer Parser::internalParseGeneratorExpression<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseGeneratorExpression<true, true>();
template ExpressionPointer Parser::internalParseGeneratorExpression<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseClassExpression<false, false, false>();
template ExpressionPointer Parser::internalParseClassExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseClassExpression<false, false, true>();
template ExpressionPointer Parser::internalParseClassExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseClassExpression<false, true, false>();
template ExpressionPointer Parser::internalParseClassExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseClassExpression<false, true, true>();
template ExpressionPointer Parser::internalParseClassExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseClassExpression<true, false, false>();
template ExpressionPointer Parser::internalParseClassExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseClassExpression<true, false, true>();
template ExpressionPointer Parser::internalParseClassExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseClassExpression<true, true, false>();
template ExpressionPointer Parser::internalParseClassExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseClassExpression<true, true, true>();
template ExpressionPointer Parser::internalParseClassExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseParenthesizedExpression<false, false, false>();
template ExpressionPointer Parser::internalParseParenthesizedExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseParenthesizedExpression<false, false, true>();
template ExpressionPointer Parser::internalParseParenthesizedExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseParenthesizedExpression<false, true, false>();
template ExpressionPointer Parser::internalParseParenthesizedExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseParenthesizedExpression<false, true, true>();
template ExpressionPointer Parser::internalParseParenthesizedExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseParenthesizedExpression<true, false, false>();
template ExpressionPointer Parser::internalParseParenthesizedExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseParenthesizedExpression<true, false, true>();
template ExpressionPointer Parser::internalParseParenthesizedExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseParenthesizedExpression<true, true, false>();
template ExpressionPointer Parser::internalParseParenthesizedExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseParenthesizedExpression<true, true, true>();
template ExpressionPointer Parser::internalParseParenthesizedExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parsePrimaryExpression<false, false, false>();
template ExpressionPointer Parser::internalParsePrimaryExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parsePrimaryExpression<false, false, true>();
template ExpressionPointer Parser::internalParsePrimaryExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parsePrimaryExpression<false, true, false>();
template ExpressionPointer Parser::internalParsePrimaryExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parsePrimaryExpression<false, true, true>();
template ExpressionPointer Parser::internalParsePrimaryExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parsePrimaryExpression<true, false, false>();
template ExpressionPointer Parser::internalParsePrimaryExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parsePrimaryExpression<true, false, true>();
template ExpressionPointer Parser::internalParsePrimaryExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parsePrimaryExpression<true, true, false>();
template ExpressionPointer Parser::internalParsePrimaryExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parsePrimaryExpression<true, true, true>();
template ExpressionPointer Parser::internalParsePrimaryExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<false, false, false>();
template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<false, false, true>();
template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<false, true, false>();
template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<false, true, true>();
template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<true, false, false>();
template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<true, false, true>();
template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<true, true, false>();
template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<true, true, true>();
template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementList Parser::parseArrayLiteralElementList<false, false, false>();
template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementList Parser::parseArrayLiteralElementList<false, false, true>();
template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementList Parser::parseArrayLiteralElementList<false, true, false>();
template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementList Parser::parseArrayLiteralElementList<false, true, true>();
template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementList Parser::parseArrayLiteralElementList<true, false, false>();
template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementList Parser::parseArrayLiteralElementList<true, false, true>();
template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementList Parser::parseArrayLiteralElementList<true, true, false>();
template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ArrayLiteralElementList Parser::parseArrayLiteralElementList<true, true, true>();
template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrayLiteral<false, false, false>();
template ExpressionPointer Parser::internalParseArrayLiteral<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrayLiteral<false, false, true>();
template ExpressionPointer Parser::internalParseArrayLiteral<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrayLiteral<false, true, false>();
template ExpressionPointer Parser::internalParseArrayLiteral<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrayLiteral<false, true, true>();
template ExpressionPointer Parser::internalParseArrayLiteral<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrayLiteral<true, false, false>();
template ExpressionPointer Parser::internalParseArrayLiteral<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrayLiteral<true, false, true>();
template ExpressionPointer Parser::internalParseArrayLiteral<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrayLiteral<true, true, false>();
template ExpressionPointer Parser::internalParseArrayLiteral<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrayLiteral<true, true, true>();
template ExpressionPointer Parser::internalParseArrayLiteral<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseObjectLiteral<false, false, false>();
template ExpressionPointer Parser::internalParseObjectLiteral<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseObjectLiteral<false, false, true>();
template ExpressionPointer Parser::internalParseObjectLiteral<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseObjectLiteral<false, true, false>();
template ExpressionPointer Parser::internalParseObjectLiteral<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseObjectLiteral<false, true, true>();
template ExpressionPointer Parser::internalParseObjectLiteral<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseObjectLiteral<true, false, false>();
template ExpressionPointer Parser::internalParseObjectLiteral<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseObjectLiteral<true, false, true>();
template ExpressionPointer Parser::internalParseObjectLiteral<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseObjectLiteral<true, true, false>();
template ExpressionPointer Parser::internalParseObjectLiteral<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseObjectLiteral<true, true, true>();
template ExpressionPointer Parser::internalParseObjectLiteral<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ast::TemplateRest Parser::parseTemplateLiteralRest<false, false, false>();
template ast::TemplateRest Parser::internalParseTemplateLiteralRest<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ast::TemplateRest Parser::parseTemplateLiteralRest<false, false, true>();
template ast::TemplateRest Parser::internalParseTemplateLiteralRest<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ast::TemplateRest Parser::parseTemplateLiteralRest<false, true, false>();
template ast::TemplateRest Parser::internalParseTemplateLiteralRest<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ast::TemplateRest Parser::parseTemplateLiteralRest<false, true, true>();
template ast::TemplateRest Parser::internalParseTemplateLiteralRest<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ast::TemplateRest Parser::parseTemplateLiteralRest<true, false, false>();
template ast::TemplateRest Parser::internalParseTemplateLiteralRest<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ast::TemplateRest Parser::parseTemplateLiteralRest<true, false, true>();
template ast::TemplateRest Parser::internalParseTemplateLiteralRest<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ast::TemplateRest Parser::parseTemplateLiteralRest<true, true, false>();
template ast::TemplateRest Parser::internalParseTemplateLiteralRest<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ast::TemplateRest Parser::parseTemplateLiteralRest<true, true, true>();
template ast::TemplateRest Parser::internalParseTemplateLiteralRest<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseTemplateLiteral<false, false, false>();
template ExpressionPointer Parser::internalParseTemplateLiteral<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseTemplateLiteral<false, false, true>();
template ExpressionPointer Parser::internalParseTemplateLiteral<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseTemplateLiteral<false, true, false>();
template ExpressionPointer Parser::internalParseTemplateLiteral<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseTemplateLiteral<false, true, true>();
template ExpressionPointer Parser::internalParseTemplateLiteral<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseTemplateLiteral<true, false, false>();
template ExpressionPointer Parser::internalParseTemplateLiteral<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseTemplateLiteral<true, false, true>();
template ExpressionPointer Parser::internalParseTemplateLiteral<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseTemplateLiteral<true, true, false>();
template ExpressionPointer Parser::internalParseTemplateLiteral<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseTemplateLiteral<true, true, true>();
template ExpressionPointer Parser::internalParseTemplateLiteral<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<false, false, false, false>();
template ExpressionPointer Parser::internalParseArrowFunction<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<false, false, false, true>();
template ExpressionPointer Parser::internalParseArrowFunction<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<false, false, true, false>();
template ExpressionPointer Parser::internalParseArrowFunction<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<false, false, true, true>();
template ExpressionPointer Parser::internalParseArrowFunction<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<false, true, false, false>();
template ExpressionPointer Parser::internalParseArrowFunction<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<false, true, false, true>();
template ExpressionPointer Parser::internalParseArrowFunction<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<false, true, true, false>();
template ExpressionPointer Parser::internalParseArrowFunction<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<false, true, true, true>();
template ExpressionPointer Parser::internalParseArrowFunction<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<true, false, false, false>();
template ExpressionPointer Parser::internalParseArrowFunction<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<true, false, false, true>();
template ExpressionPointer Parser::internalParseArrowFunction<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<true, false, true, false>();
template ExpressionPointer Parser::internalParseArrowFunction<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<true, false, true, true>();
template ExpressionPointer Parser::internalParseArrowFunction<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<true, true, false, false>();
template ExpressionPointer Parser::internalParseArrowFunction<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<true, true, false, true>();
template ExpressionPointer Parser::internalParseArrowFunction<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<true, true, true, false>();
template ExpressionPointer Parser::internalParseArrowFunction<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseArrowFunction<true, true, true, true>();
template ExpressionPointer Parser::internalParseArrowFunction<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseYieldExpression<false, false, false>();
template ExpressionPointer Parser::internalParseYieldExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseYieldExpression<false, false, true>();
template ExpressionPointer Parser::internalParseYieldExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseYieldExpression<false, true, false>();
template ExpressionPointer Parser::internalParseYieldExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseYieldExpression<false, true, true>();
template ExpressionPointer Parser::internalParseYieldExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseYieldExpression<true, false, false>();
template ExpressionPointer Parser::internalParseYieldExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseYieldExpression<true, false, true>();
template ExpressionPointer Parser::internalParseYieldExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseYieldExpression<true, true, false>();
template ExpressionPointer Parser::internalParseYieldExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseYieldExpression<true, true, true>();
template ExpressionPointer Parser::internalParseYieldExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseLeftHandSideExpression<false, false, false>();
template ExpressionPointer Parser::internalParseLeftHandSideExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseLeftHandSideExpression<false, false, true>();
template ExpressionPointer Parser::internalParseLeftHandSideExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseLeftHandSideExpression<false, true, false>();
template ExpressionPointer Parser::internalParseLeftHandSideExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseLeftHandSideExpression<false, true, true>();
template ExpressionPointer Parser::internalParseLeftHandSideExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseLeftHandSideExpression<true, false, false>();
template ExpressionPointer Parser::internalParseLeftHandSideExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseLeftHandSideExpression<true, false, true>();
template ExpressionPointer Parser::internalParseLeftHandSideExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseLeftHandSideExpression<true, true, false>();
template ExpressionPointer Parser::internalParseLeftHandSideExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseLeftHandSideExpression<true, true, true>();
template ExpressionPointer Parser::internalParseLeftHandSideExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<false, false, false, false>();
template ExpressionPointer Parser::internalParseConditionalExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<false, false, false, true>();
template ExpressionPointer Parser::internalParseConditionalExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<false, false, true, false>();
template ExpressionPointer Parser::internalParseConditionalExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<false, false, true, true>();
template ExpressionPointer Parser::internalParseConditionalExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<false, true, false, false>();
template ExpressionPointer Parser::internalParseConditionalExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<false, true, false, true>();
template ExpressionPointer Parser::internalParseConditionalExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<false, true, true, false>();
template ExpressionPointer Parser::internalParseConditionalExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<false, true, true, true>();
template ExpressionPointer Parser::internalParseConditionalExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<true, false, false, false>();
template ExpressionPointer Parser::internalParseConditionalExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<true, false, false, true>();
template ExpressionPointer Parser::internalParseConditionalExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<true, false, true, false>();
template ExpressionPointer Parser::internalParseConditionalExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<true, false, true, true>();
template ExpressionPointer Parser::internalParseConditionalExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<true, true, false, false>();
template ExpressionPointer Parser::internalParseConditionalExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<true, true, false, true>();
template ExpressionPointer Parser::internalParseConditionalExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<true, true, true, false>();
template ExpressionPointer Parser::internalParseConditionalExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseConditionalExpression<true, true, true, true>();
template ExpressionPointer Parser::internalParseConditionalExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<false, false, false, false>();
template ExpressionPointer Parser::internalParseAssignmentExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<false, false, false, true>();
template ExpressionPointer Parser::internalParseAssignmentExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<false, false, true, false>();
template ExpressionPointer Parser::internalParseAssignmentExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<false, false, true, true>();
template ExpressionPointer Parser::internalParseAssignmentExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<false, true, false, false>();
template ExpressionPointer Parser::internalParseAssignmentExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<false, true, false, true>();
template ExpressionPointer Parser::internalParseAssignmentExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<false, true, true, false>();
template ExpressionPointer Parser::internalParseAssignmentExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<false, true, true, true>();
template ExpressionPointer Parser::internalParseAssignmentExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<true, false, false, false>();
template ExpressionPointer Parser::internalParseAssignmentExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<true, false, false, true>();
template ExpressionPointer Parser::internalParseAssignmentExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<true, false, true, false>();
template ExpressionPointer Parser::internalParseAssignmentExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<true, false, true, true>();
template ExpressionPointer Parser::internalParseAssignmentExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<true, true, false, false>();
template ExpressionPointer Parser::internalParseAssignmentExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<true, true, false, true>();
template ExpressionPointer Parser::internalParseAssignmentExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<true, true, true, false>();
template ExpressionPointer Parser::internalParseAssignmentExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseAssignmentExpression<true, true, true, true>();
template ExpressionPointer Parser::internalParseAssignmentExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<false, false, false, false>();
template ExpressionPointer Parser::internalParseExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<false, false, false, true>();
template ExpressionPointer Parser::internalParseExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<false, false, true, false>();
template ExpressionPointer Parser::internalParseExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<false, false, true, true>();
template ExpressionPointer Parser::internalParseExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<false, true, false, false>();
template ExpressionPointer Parser::internalParseExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<false, true, false, true>();
template ExpressionPointer Parser::internalParseExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<false, true, true, false>();
template ExpressionPointer Parser::internalParseExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<false, true, true, true>();
template ExpressionPointer Parser::internalParseExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<true, false, false, false>();
template ExpressionPointer Parser::internalParseExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<true, false, false, true>();
template ExpressionPointer Parser::internalParseExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<true, false, true, false>();
template ExpressionPointer Parser::internalParseExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<true, false, true, true>();
template ExpressionPointer Parser::internalParseExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<true, true, false, false>();
template ExpressionPointer Parser::internalParseExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<true, true, false, true>();
template ExpressionPointer Parser::internalParseExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<true, true, true, false>();
template ExpressionPointer Parser::internalParseExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
template ExpressionPointer Parser::parseExpression<true, true, true, true>();
template ExpressionPointer Parser::internalParseExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
}
}
