// automatically generated from javascript_tasklets/parser/parser_imp.grammar
#line 1 "javascript_tasklets/parser/parser_imp.grammar"
              
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
// command: peg_parser_generator parser_imp.grammar

#line 29 "javascript_tasklets/parser/parser_imp.h"
#ifndef JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_
#define JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_

#include <utility>
#include <cstddef>
#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <list>
#include <cassert>
#line 29 "javascript_tasklets/parser/parser_imp.grammar"
             
#include "../string.h"

#line 46 "javascript_tasklets/parser/parser_imp.h"

namespace javascript_tasklets
{
namespace parser
{
class Parser final
{
    Parser(const Parser &) = delete;
    Parser &operator=(const Parser &) = delete;

private:
    struct RuleResult final
    {
        std::size_t location;
        std::size_t endLocation;
        bool isSuccess;
        constexpr RuleResult() noexcept : location(std::string::npos),
                                          endLocation(0),
                                          isSuccess(false)
        {
        }
        constexpr RuleResult(std::size_t location, std::size_t endLocation, bool success) noexcept
            : location(location),
              endLocation(endLocation),
              isSuccess(success)
        {
        }
        constexpr bool empty() const
        {
            return location == std::string::npos;
        }
        constexpr bool success() const
        {
            return !empty() && isSuccess;
        }
        constexpr bool fail() const
        {
            return !empty() && !isSuccess;
        }
    };
    struct Results final
    {
        RuleResult resultTokenizerHexDigitValue;
        RuleResult resultTokenizerLineTerminatorSequence;
        RuleResult resultTokenizerMultiLineComment;
        RuleResult resultTokenizerMultiLineCommentNoLineTerminator;
        RuleResult resultTokenizerSingleLineComment;
        RuleResult resultTokenizerUnicodeEscapeSequence;
        RuleResult resultTokenizerUnicodeEscapeOrChar;
        RuleResult resultTokenizerIdentifierName;
        RuleResult resultTokenizerEscapelessIdentifierName;
        RuleResult resultTokenizerAwait;
        RuleResult resultTokenizerBreak;
        RuleResult resultTokenizerCase;
        RuleResult resultTokenizerCatch;
        RuleResult resultTokenizerClass;
        RuleResult resultTokenizerConst;
        RuleResult resultTokenizerContinue;
        RuleResult resultTokenizerDebugger;
        RuleResult resultTokenizerDefault;
        RuleResult resultTokenizerDelete;
        RuleResult resultTokenizerDo;
        RuleResult resultTokenizerElse;
        RuleResult resultTokenizerEnum;
        RuleResult resultTokenizerExport;
        RuleResult resultTokenizerExtends;
        RuleResult resultTokenizerFalse;
        RuleResult resultTokenizerFinally;
        RuleResult resultTokenizerFor;
        RuleResult resultTokenizerFunction;
        RuleResult resultTokenizerIf;
        RuleResult resultTokenizerImplements;
        RuleResult resultTokenizerImport;
        RuleResult resultTokenizerIn;
        RuleResult resultTokenizerInstanceOf;
        RuleResult resultTokenizerInterface;
        RuleResult resultTokenizerNew;
        RuleResult resultTokenizerNull;
        RuleResult resultTokenizerPackage;
        RuleResult resultTokenizerPrivate;
        RuleResult resultTokenizerProtected;
        RuleResult resultTokenizerPublic;
        RuleResult resultTokenizerReturn;
        RuleResult resultTokenizerSuper;
        RuleResult resultTokenizerSwitch;
        RuleResult resultTokenizerThis;
        RuleResult resultTokenizerThrow;
        RuleResult resultTokenizerTrue;
        RuleResult resultTokenizerTry;
        RuleResult resultTokenizerTypeOf;
        RuleResult resultTokenizerVar;
        RuleResult resultTokenizerVoid;
        RuleResult resultTokenizerWhile;
        RuleResult resultTokenizerWith;
        RuleResult resultTokenizerYield;
        RuleResult resultTokenizerBooleanLiteral;
    };
    struct ResultsChunk final
    {
        static constexpr std::size_t allocated = 0x100;
        Results values[allocated];
        std::size_t used = 0;
    };

public:
    struct ParseError : public std::runtime_error
    {
        std::size_t location;
        const char *message;
        static std::string makeWhatString(std::size_t location, const char *message)
        {
            std::ostringstream ss;
            ss << "error at " << location << ": " << message;
            return ss.str();
        }
        ParseError(std::size_t location, const char *message)
            : runtime_error(makeWhatString(location, message)), location(location), message(message)
        {
        }
    };

private:
    std::vector<Results *> resultsPointers;
    std::list<ResultsChunk> resultsChunks;
    Results eofResults;
    const std::shared_ptr<const char32_t> source;
    const std::size_t sourceSize;
    std::size_t errorLocation = 0;
    std::size_t errorInputEndLocation = 0;
    const char *errorMessage = "no error";

private:
    Results &getResults(std::size_t position)
    {
        if(position >= sourceSize)
            return eofResults;
        Results *&resultsPointer = resultsPointers[position];
        if(!resultsPointer)
        {
            if(resultsChunks.empty() || resultsChunks.back().used >= ResultsChunk::allocated)
            {
                resultsChunks.emplace_back();
            }
            resultsPointer = &resultsChunks.back().values[resultsChunks.back().used++];
        }
        return *resultsPointer;
    }
    RuleResult makeFail(std::size_t location,
                        std::size_t inputEndLocation,
                        const char *message,
                        bool isRequiredForSuccess)
    {
        if(isRequiredForSuccess && errorInputEndLocation <= inputEndLocation)
        {
            errorLocation = location;
            errorInputEndLocation = inputEndLocation;
            errorMessage = message;
        }
        return RuleResult(location, inputEndLocation, false);
    }
    RuleResult makeFail(std::size_t inputEndLocation,
                        const char *message,
                        bool isRequiredForSuccess)
    {
        return makeFail(inputEndLocation, inputEndLocation, message, isRequiredForSuccess);
    }
    static RuleResult makeSuccess(std::size_t location, std::size_t inputEndLocation)
    {
        assert(location != std::string::npos);
        return RuleResult(location, inputEndLocation, true);
    }
    static RuleResult makeSuccess(std::size_t inputEndLocation)
    {
        assert(inputEndLocation != std::string::npos);
        return RuleResult(inputEndLocation, inputEndLocation, true);
    }
    static std::pair<std::shared_ptr<const char32_t>, std::size_t> makeSource(
        std::u32string source);
    static std::pair<std::shared_ptr<const char32_t>, std::size_t> makeSource(
        const char *source, std::size_t sourceSize);

public:
    Parser(std::pair<std::shared_ptr<const char32_t>, std::size_t> source)
        : Parser(std::move(std::get<0>(source)), std::get<1>(source))
    {
    }
    Parser(std::shared_ptr<const char32_t> source, std::size_t sourceSize);
    Parser(std::u32string source);
    Parser(const char *source, std::size_t sourceSize);
    Parser(const char32_t *source, std::size_t sourceSize);
    Parser(const std::string &source) : Parser(source.data(), source.size())
    {
    }

public:
    unsigned parseTokenizerHexDigitValue();
    char32_t parseTokenizerLineTerminatorSequence();
    void parseTokenizerComment();
    void parseTokenizerMultiLineComment();
    void parseTokenizerMultiLineCommentNoLineTerminator();
    void parseTokenizerSingleLineComment();
    char32_t parseTokenizerUnicodeEscapeSequence();
    char32_t parseTokenizerUnicodeEscapeOrChar();
    javascript_tasklets::String parseTokenizerIdentifierName();
    javascript_tasklets::String parseTokenizerEscapelessIdentifierName();
    void parseTokenizerReservedWord();
    void parseTokenizerAwait();
    void parseTokenizerBreak();
    void parseTokenizerCase();
    void parseTokenizerCatch();
    void parseTokenizerClass();
    void parseTokenizerConst();
    void parseTokenizerContinue();
    void parseTokenizerDebugger();
    void parseTokenizerDefault();
    void parseTokenizerDelete();
    void parseTokenizerDo();
    void parseTokenizerElse();
    void parseTokenizerEnum();
    void parseTokenizerExport();
    void parseTokenizerExtends();
    void parseTokenizerFalse();
    void parseTokenizerFinally();
    void parseTokenizerFor();
    void parseTokenizerFunction();
    void parseTokenizerIf();
    void parseTokenizerImplements();
    void parseTokenizerImport();
    void parseTokenizerIn();
    void parseTokenizerInstanceOf();
    void parseTokenizerInterface();
    void parseTokenizerNew();
    void parseTokenizerNull();
    void parseTokenizerPackage();
    void parseTokenizerPrivate();
    void parseTokenizerProtected();
    void parseTokenizerPublic();
    void parseTokenizerReturn();
    void parseTokenizerSuper();
    void parseTokenizerSwitch();
    void parseTokenizerThis();
    void parseTokenizerThrow();
    void parseTokenizerTrue();
    void parseTokenizerTry();
    void parseTokenizerTypeOf();
    void parseTokenizerVar();
    void parseTokenizerVoid();
    void parseTokenizerWhile();
    void parseTokenizerWith();
    void parseTokenizerYield();
    void parseTokenizerKeyword();
    void parseTokenizerFutureReservedWord();
    void parseTokenizerNullLiteral();
    bool parseTokenizerBooleanLiteral();

private:
    unsigned internalParseTokenizerHexDigitValue(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerLineTerminatorSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerMultiLineComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerMultiLineCommentNoLineTerminator(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerSingleLineComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerUnicodeEscapeSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerUnicodeEscapeOrChar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    javascript_tasklets::String internalParseTokenizerIdentifierName(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    javascript_tasklets::String internalParseTokenizerEscapelessIdentifierName(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerReservedWord(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerAwait(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerBreak(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerCase(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerCatch(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerClass(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerConst(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerContinue(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerDebugger(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerDefault(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerDelete(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerDo(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerElse(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerEnum(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerExport(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerExtends(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerFalse(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerFinally(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerFor(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerFunction(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerIf(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerImplements(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerImport(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerIn(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerInstanceOf(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerInterface(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerNew(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerNull(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPackage(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPrivate(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerProtected(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPublic(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerReturn(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerSuper(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerSwitch(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerThis(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerThrow(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerTrue(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerTry(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerTypeOf(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerVar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerVoid(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerWhile(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerWith(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerYield(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerKeyword(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerFutureReservedWord(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerNullLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    bool internalParseTokenizerBooleanLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
};
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_ */
