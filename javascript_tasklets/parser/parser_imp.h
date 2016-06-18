// automatically generated from parser_imp.grammar
#line 1 "parser_imp.grammar"
              
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

#line 29 "parser_imp.h"
#ifndef PARSER_IMP_H_
#define PARSER_IMP_H_

#include <utility>
#include <cstddef>
#include <string>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <list>
#include <cassert>

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
        RuleResult resultUnicodeCategoryLu;
        RuleResult resultUnicodeCategoryLl;
        RuleResult resultUnicodeCategoryLt;
        RuleResult resultUnicodeCategoryLm;
        RuleResult resultUnicodeCategoryLo;
        RuleResult resultUnicodeCategoryNl;
        RuleResult resultUnicodeCategoryMn;
        RuleResult resultUnicodePropertyPattern_Syntax;
        RuleResult resultUnicodeCategoryMc;
        RuleResult resultUnicodeCategoryNd;
        RuleResult resultUnicodeCategoryPc;
        RuleResult resultUnicodeCategoryZs;
        RuleResult resultUnicodePropertyPattern_White_Space;
        RuleResult resultUnicodePropertyOther_ID_Start;
        RuleResult resultUnicodePropertyOther_ID_Continue;
        RuleResult resultUnicodePropertyIdStart;
        RuleResult resultUnicodePropertyIdContinue;
        RuleResult resultECMAScriptIdStart;
        RuleResult resultECMAScriptIdContinue;
        RuleResult resultECMAScriptWhiteSpace;
        RuleResult resultECMAScriptLineTerminator;
        RuleResult resultECMAScriptHexDigit;
        RuleResult resultECMAScriptDecimalDigit;
        RuleResult resultECMAScriptBinaryDigit;
        RuleResult resultECMAScriptOctalDigit;
        RuleResult resultECMAScriptNonZeroDigit;
        RuleResult resultECMAScriptExponentIndicator;
        RuleResult resultECMAScriptDigitValue;
        RuleResult resultECMAScriptSingleEscapeCharacter;
        RuleResult resultECMAScriptEscapeCharacter;
        RuleResult resultECMAScriptNonEscapeCharacter;
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
    char32_t parseUnicodeCategoryLu();
    char32_t parseUnicodeCategoryLl();
    char32_t parseUnicodeCategoryLt();
    char32_t parseUnicodeCategoryLm();
    char32_t parseUnicodeCategoryLo();
    char32_t parseUnicodeCategoryNl();
    char32_t parseUnicodeCategoryMn();
    char32_t parseUnicodePropertyPattern_Syntax();
    char32_t parseUnicodeCategoryMc();
    char32_t parseUnicodeCategoryNd();
    char32_t parseUnicodeCategoryPc();
    char32_t parseUnicodeCategoryZs();
    char32_t parseUnicodePropertyPattern_White_Space();
    char32_t parseUnicodePropertyOther_ID_Start();
    char32_t parseUnicodePropertyOther_ID_Continue();
    char32_t parseUnicodePropertyIdStart();
    char32_t parseUnicodePropertyIdContinue();
    char32_t parseECMAScriptIdStart();
    char32_t parseECMAScriptIdContinue();
    char32_t parseECMAScriptWhiteSpace();
    char32_t parseECMAScriptLineTerminator();
    char32_t parseECMAScriptHexDigit();
    char32_t parseECMAScriptDecimalDigit();
    char32_t parseECMAScriptBinaryDigit();
    char32_t parseECMAScriptOctalDigit();
    char32_t parseECMAScriptNonZeroDigit();
    char32_t parseECMAScriptExponentIndicator();
    unsigned parseECMAScriptDigitValue();
    char32_t parseECMAScriptSingleEscapeCharacter();
    char32_t parseECMAScriptEscapeCharacter();
    char32_t parseECMAScriptNonEscapeCharacter();

private:
    RuleResult internalParseUnicodeCategoryLu(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryLu(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryLl(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryLl(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryLt(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryLt(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryLm(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryLm(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryLo(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryLo(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryNl(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryNl(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryMn(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryMn(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodePropertyPattern_Syntax(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodePropertyPattern_Syntax(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryMc(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryMc(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryNd(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryNd(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryPc(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryPc(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodeCategoryZs(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodeCategoryZs(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodePropertyPattern_White_Space(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodePropertyPattern_White_Space(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodePropertyOther_ID_Start(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodePropertyOther_ID_Start(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodePropertyOther_ID_Continue(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodePropertyOther_ID_Continue(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodePropertyIdStart(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodePropertyIdStart(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseUnicodePropertyIdContinue(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateUnicodePropertyIdContinue(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptIdStart(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptIdStart(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptIdContinue(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptIdContinue(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptWhiteSpace(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptWhiteSpace(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptLineTerminator(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptLineTerminator(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptHexDigit(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptHexDigit(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptDecimalDigit(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptDecimalDigit(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptBinaryDigit(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptBinaryDigit(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptOctalDigit(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptOctalDigit(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptNonZeroDigit(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptNonZeroDigit(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptExponentIndicator(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptExponentIndicator(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptDigitValue(std::size_t startLocation, bool isRequiredForSuccess);
    unsigned internalEvaluateECMAScriptDigitValue(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptSingleEscapeCharacter(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptSingleEscapeCharacter(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptEscapeCharacter(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptEscapeCharacter(std::size_t startLocation, RuleResult &ruleResult);
    RuleResult internalParseECMAScriptNonEscapeCharacter(std::size_t startLocation, bool isRequiredForSuccess);
    char32_t internalEvaluateECMAScriptNonEscapeCharacter(std::size_t startLocation, RuleResult &ruleResult);
};
}

#endif /* PARSER_IMP_H_ */
