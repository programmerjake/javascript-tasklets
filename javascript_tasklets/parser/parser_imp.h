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
#line 29 "javascript_tasklets/parser/parser_imp.peg"
             
#include "../string.h"

namespace javascript_tasklets
{
namespace parser
{
struct RawAndCookedString final
{
    String raw;
    String cooked;
    RawAndCookedString() : raw(), cooked()
    {
    }
    RawAndCookedString(String raw) : raw(raw), cooked(std::move(raw))
    {
    }
    RawAndCookedString(String raw, String cooked) : raw(std::move(raw)), cooked(std::move(cooked))
    {
    }
};

struct RawStringAndChar final
{
    String raw;
    char32_t ch;
    RawStringAndChar() : raw(), ch()
    {
    }
    RawStringAndChar(String raw, char32_t ch) : raw(std::move(raw)), ch(ch)
    {
    }
};

struct BodyAndFlags final
{
    String body;
    String flags;
    BodyAndFlags() : body(), flags()
    {
    }
    BodyAndFlags(String body, String flags) : body(std::move(body)), flags(std::move(flags))
    {
    }
};
}
}

#line 91 "javascript_tasklets/parser/parser_imp.h"

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
        RuleResult resultTokenizerEscapelessIdentifierStart;
        RuleResult resultTokenizerEscapelessIdentifierPart;
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
        RuleResult resultTokenizerFutureReservedWord[2][2];
        RuleResult resultTokenizerBooleanLiteral;
        RuleResult resultTokenizerPeriod;
        RuleResult resultTokenizerEllipsis;
        RuleResult resultTokenizerLAngle;
        RuleResult resultTokenizerRAngle;
        RuleResult resultTokenizerLAngleEqual;
        RuleResult resultTokenizerRAngleEqual;
        RuleResult resultTokenizerEqualEqual;
        RuleResult resultTokenizerEMarkEqual;
        RuleResult resultTokenizerEqualEqualEqual;
        RuleResult resultTokenizerEMarkEqualEqual;
        RuleResult resultTokenizerPlus;
        RuleResult resultTokenizerMinus;
        RuleResult resultTokenizerStar;
        RuleResult resultTokenizerStarStar;
        RuleResult resultTokenizerPercent;
        RuleResult resultTokenizerPlusPlus;
        RuleResult resultTokenizerMinusMinus;
        RuleResult resultTokenizerLAngleLAngle;
        RuleResult resultTokenizerRAngleRAngle;
        RuleResult resultTokenizerRAngleRAngleRAngle;
        RuleResult resultTokenizerAmp;
        RuleResult resultTokenizerPipe;
        RuleResult resultTokenizerCaret;
        RuleResult resultTokenizerEMark;
        RuleResult resultTokenizerAmpAmp;
        RuleResult resultTokenizerPipePipe;
        RuleResult resultTokenizerEqual;
        RuleResult resultTokenizerPlusEqual;
        RuleResult resultTokenizerMinusEqual;
        RuleResult resultTokenizerStarEqual;
        RuleResult resultTokenizerPercentEqual;
        RuleResult resultTokenizerLAngleLAngleEqual;
        RuleResult resultTokenizerRAngleRAngleEqual;
        RuleResult resultTokenizerRAngleRAngleRAngleEqual;
        RuleResult resultTokenizerAmpEqual;
        RuleResult resultTokenizerPipeEqual;
        RuleResult resultTokenizerCaretEqual;
        RuleResult resultTokenizerArrow;
        RuleResult resultTokenizerFSlash;
        RuleResult resultTokenizerFSlashEqual;
        RuleResult resultTokenizerNumericLiteral;
        RuleResult resultTokenizerDecimalLiteral;
        RuleResult resultTokenizerDecimalIntegerLiteral;
        RuleResult resultTokenizerDecimalDigits;
        RuleResult resultTokenizerExponentPart;
        RuleResult resultTokenizerSignedInteger;
        RuleResult resultTokenizerBinaryIntegerLiteral;
        RuleResult resultTokenizerBinaryDigits;
        RuleResult resultTokenizerOctalIntegerLiteral;
        RuleResult resultTokenizerOctalDigits;
        RuleResult resultTokenizerHexIntegerLiteral;
        RuleResult resultTokenizerHexDigits;
        RuleResult resultTokenizerStringLiteral;
        RuleResult resultTokenizerSingleStringCharacters;
        RuleResult resultTokenizerDoubleStringCharacters;
        RuleResult resultTokenizerSingleStringCharacter;
        RuleResult resultTokenizerDoubleStringCharacter;
        RuleResult resultTokenizerLineContinuation;
        RuleResult resultTokenizerEscapeSequence;
        RuleResult resultTokenizerHexEscapeSequence;
        RuleResult resultTokenizerCharacterEscapeSequence;
        RuleResult resultTokenizerSingleEscapeCharacter;
        RuleResult resultTokenizerNonEscapeCharacter;
        RuleResult resultTokenizerRegularExpressionLiteral;
        RuleResult resultTokenizerRegularExpressionBody;
        RuleResult resultTokenizerRegularExpressionChar;
        RuleResult resultTokenizerRegularExpressionBackslashSequence;
        RuleResult resultTokenizerRegularExpressionClass;
        RuleResult resultTokenizerRegularExpressionClassChar;
        RuleResult resultTokenizerRegularExpressionFlags;
        RuleResult resultTokenizerNoSubstitutionTemplate;
        RuleResult resultTokenizerTemplateHead;
        RuleResult resultTokenizerTemplateMiddle;
        RuleResult resultTokenizerTemplateTail;
        RuleResult resultTokenizerTemplateCharacter;
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
    RawStringAndChar parseTokenizerUnicodeEscapeSequence();
    char32_t parseTokenizerUnicodeEscapeOrChar();
    char32_t parseTokenizerEscapelessIdentifierStart();
    char32_t parseTokenizerEscapelessIdentifierPart();
    String parseTokenizerIdentifierName();
    String parseTokenizerEscapelessIdentifierName();
    template <bool isModule, bool isStrict>
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
    template <bool isModule, bool isStrict>
    void parseTokenizerFutureReservedWord();
    void parseTokenizerNullLiteral();
    bool parseTokenizerBooleanLiteral();
    void parseTokenizerLBrace();
    void parseTokenizerRBrace();
    void parseTokenizerLParen();
    void parseTokenizerRParen();
    void parseTokenizerLBracket();
    void parseTokenizerRBracket();
    void parseTokenizerPeriod();
    void parseTokenizerEllipsis();
    void parseTokenizerSemicolon();
    void parseTokenizerComma();
    void parseTokenizerLAngle();
    void parseTokenizerRAngle();
    void parseTokenizerLAngleEqual();
    void parseTokenizerRAngleEqual();
    void parseTokenizerEqualEqual();
    void parseTokenizerEMarkEqual();
    void parseTokenizerEqualEqualEqual();
    void parseTokenizerEMarkEqualEqual();
    void parseTokenizerPlus();
    void parseTokenizerMinus();
    void parseTokenizerStar();
    void parseTokenizerStarStar();
    void parseTokenizerPercent();
    void parseTokenizerPlusPlus();
    void parseTokenizerMinusMinus();
    void parseTokenizerLAngleLAngle();
    void parseTokenizerRAngleRAngle();
    void parseTokenizerRAngleRAngleRAngle();
    void parseTokenizerAmp();
    void parseTokenizerPipe();
    void parseTokenizerCaret();
    void parseTokenizerEMark();
    void parseTokenizerTilde();
    void parseTokenizerAmpAmp();
    void parseTokenizerPipePipe();
    void parseTokenizerQMark();
    void parseTokenizerColon();
    void parseTokenizerEqual();
    void parseTokenizerPlusEqual();
    void parseTokenizerMinusEqual();
    void parseTokenizerStarEqual();
    void parseTokenizerPercentEqual();
    void parseTokenizerLAngleLAngleEqual();
    void parseTokenizerRAngleRAngleEqual();
    void parseTokenizerRAngleRAngleRAngleEqual();
    void parseTokenizerAmpEqual();
    void parseTokenizerPipeEqual();
    void parseTokenizerCaretEqual();
    void parseTokenizerArrow();
    void parseTokenizerFSlash();
    void parseTokenizerFSlashEqual();
    void parseTokenizerPunctuator();
    void parseTokenizerDivPunctuator();
    void parseTokenizerRightBracePunctuator();
    String parseTokenizerNumericLiteral();
    String parseTokenizerDecimalLiteral();
    String parseTokenizerDecimalIntegerLiteral();
    String parseTokenizerDecimalDigits();
    String parseTokenizerExponentPart();
    String parseTokenizerSignedInteger();
    String parseTokenizerBinaryIntegerLiteral();
    String parseTokenizerBinaryDigits();
    String parseTokenizerOctalIntegerLiteral();
    String parseTokenizerOctalDigits();
    String parseTokenizerHexIntegerLiteral();
    String parseTokenizerHexDigits();
    String parseTokenizerStringLiteral();
    String parseTokenizerSingleStringCharacters();
    String parseTokenizerDoubleStringCharacters();
    String parseTokenizerSingleStringCharacter();
    String parseTokenizerDoubleStringCharacter();
    String parseTokenizerLineContinuation();
    RawAndCookedString parseTokenizerEscapeSequence();
    RawStringAndChar parseTokenizerHexEscapeSequence();
    RawAndCookedString parseTokenizerCharacterEscapeSequence();
    RawAndCookedString parseTokenizerSingleEscapeCharacter();
    char32_t parseTokenizerNonEscapeCharacter();
    BodyAndFlags parseTokenizerRegularExpressionLiteral();
    String parseTokenizerRegularExpressionBody();
    String parseTokenizerRegularExpressionChar();
    String parseTokenizerRegularExpressionBackslashSequence();
    String parseTokenizerRegularExpressionClass();
    String parseTokenizerRegularExpressionClassChar();
    String parseTokenizerRegularExpressionFlags();
    RawAndCookedString parseTokenizerNoSubstitutionTemplate();
    RawAndCookedString parseTokenizerTemplateHead();
    RawAndCookedString parseTokenizerTemplateMiddle();
    RawAndCookedString parseTokenizerTemplateTail();
    RawAndCookedString parseTokenizerTemplateCharacter();

private:
    unsigned internalParseTokenizerHexDigitValue(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerLineTerminatorSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerMultiLineComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerMultiLineCommentNoLineTerminator(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerSingleLineComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawStringAndChar internalParseTokenizerUnicodeEscapeSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerUnicodeEscapeOrChar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerEscapelessIdentifierStart(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerEscapelessIdentifierPart(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerIdentifierName(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerEscapelessIdentifierName(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict>
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
    template <bool isModule, bool isStrict>
    void internalParseTokenizerFutureReservedWord(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerNullLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    bool internalParseTokenizerBooleanLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerLBrace(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRBrace(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerLParen(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRParen(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerLBracket(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRBracket(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPeriod(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerEllipsis(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerSemicolon(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerComma(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerLAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerLAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerEqualEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerEMarkEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerEqualEqualEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerEMarkEqualEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPlus(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerMinus(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerStar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerStarStar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPercent(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPlusPlus(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerMinusMinus(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerLAngleLAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRAngleRAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRAngleRAngleRAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerAmp(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPipe(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerCaret(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerEMark(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerTilde(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerAmpAmp(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPipePipe(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerQMark(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerColon(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPlusEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerMinusEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerStarEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPercentEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerLAngleLAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRAngleRAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRAngleRAngleRAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerAmpEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPipeEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerCaretEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerArrow(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerFSlash(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerFSlashEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPunctuator(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerDivPunctuator(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerRightBracePunctuator(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerNumericLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerDecimalLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerDecimalIntegerLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerDecimalDigits(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerExponentPart(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerSignedInteger(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerBinaryIntegerLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerBinaryDigits(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerOctalIntegerLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerOctalDigits(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerHexIntegerLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerHexDigits(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerStringLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerSingleStringCharacters(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerDoubleStringCharacters(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerSingleStringCharacter(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerDoubleStringCharacter(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerLineContinuation(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenizerEscapeSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawStringAndChar internalParseTokenizerHexEscapeSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenizerCharacterEscapeSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenizerSingleEscapeCharacter(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerNonEscapeCharacter(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    BodyAndFlags internalParseTokenizerRegularExpressionLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerRegularExpressionBody(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerRegularExpressionChar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerRegularExpressionBackslashSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerRegularExpressionClass(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerRegularExpressionClassChar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenizerRegularExpressionFlags(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenizerNoSubstitutionTemplate(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenizerTemplateHead(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenizerTemplateMiddle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenizerTemplateTail(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenizerTemplateCharacter(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
};

extern template void Parser::parseTokenizerReservedWord<false, false>();
extern template void Parser::internalParseTokenizerReservedWord<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerReservedWord<false, true>();
extern template void Parser::internalParseTokenizerReservedWord<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerReservedWord<true, false>();
extern template void Parser::internalParseTokenizerReservedWord<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerReservedWord<true, true>();
extern template void Parser::internalParseTokenizerReservedWord<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerFutureReservedWord<false, false>();
extern template void Parser::internalParseTokenizerFutureReservedWord<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerFutureReservedWord<false, true>();
extern template void Parser::internalParseTokenizerFutureReservedWord<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerFutureReservedWord<true, false>();
extern template void Parser::internalParseTokenizerFutureReservedWord<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerFutureReservedWord<true, true>();
extern template void Parser::internalParseTokenizerFutureReservedWord<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_ */
