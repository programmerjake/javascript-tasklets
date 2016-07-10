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
#include <memory>
#include <vector>
#include "ast/node.h"
#include "ast/expression.h"
#include "ast/literal.h"
#include "ast/arena.h"

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

typedef ast::Node *NodePointer;
typedef ast::Expression *ExpressionPointer;
typedef ast::ArrayLiteralElement *ArrayLiteralElementPointer;
typedef std::vector<ast::ArrayLiteralElement *> ArrayLiteralElementList;
}
}

#line 102 "javascript_tasklets/parser/parser_imp.h"

namespace javascript_tasklets
{
namespace parser
{
class Parser final
{
    Parser(const Parser &) = delete;
    Parser &operator=(const Parser &) = delete;
#line 89 "javascript_tasklets/parser/parser_imp.peg"
            
public:
    ast::Arena arena = ast::Arena();

#line 117 "javascript_tasklets/parser/parser_imp.h"

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
        RuleResult resultTokenizerMultiLineComment[2];
        RuleResult resultTokenizerSingleLineComment;
        RuleResult resultTokenizerTokenSeperator[2];
        RuleResult resultTokenizerTokenSeperatorWithLineTerminator;
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
        RuleResult resultIdentifierName;
        RuleResult resultTokenNoSubstitutionTemplate;
        RuleResult resultTokenTemplateHead;
        RuleResult resultTokenTemplateMiddle;
        RuleResult resultTokenTemplateTail;
        RuleResult resultTokenAwait;
        RuleResult resultTokenBreak;
        RuleResult resultTokenCase;
        RuleResult resultTokenCatch;
        RuleResult resultTokenClass;
        RuleResult resultTokenConst;
        RuleResult resultTokenContinue;
        RuleResult resultTokenDebugger;
        RuleResult resultTokenDefault;
        RuleResult resultTokenDelete;
        RuleResult resultTokenDo;
        RuleResult resultTokenElse;
        RuleResult resultTokenEnum;
        RuleResult resultTokenExport;
        RuleResult resultTokenExtends;
        RuleResult resultTokenFalse;
        RuleResult resultTokenFinally;
        RuleResult resultTokenFor;
        RuleResult resultTokenFunction;
        RuleResult resultTokenIf;
        RuleResult resultTokenImplements;
        RuleResult resultTokenImport;
        RuleResult resultTokenIn;
        RuleResult resultTokenInstanceOf;
        RuleResult resultTokenInterface;
        RuleResult resultTokenNew;
        RuleResult resultTokenNull;
        RuleResult resultTokenPackage;
        RuleResult resultTokenPrivate;
        RuleResult resultTokenProtected;
        RuleResult resultTokenPublic;
        RuleResult resultTokenReturn;
        RuleResult resultTokenSuper;
        RuleResult resultTokenSwitch;
        RuleResult resultTokenThis;
        RuleResult resultTokenThrow;
        RuleResult resultTokenTrue;
        RuleResult resultTokenTry;
        RuleResult resultTokenTypeOf;
        RuleResult resultTokenVar;
        RuleResult resultTokenVoid;
        RuleResult resultTokenWhile;
        RuleResult resultTokenWith;
        RuleResult resultTokenYield;
        RuleResult resultTokenLBrace;
        RuleResult resultTokenRBrace;
        RuleResult resultTokenLParen;
        RuleResult resultTokenRParen;
        RuleResult resultTokenLBracket;
        RuleResult resultTokenRBracket;
        RuleResult resultTokenPeriod;
        RuleResult resultTokenEllipsis;
        RuleResult resultTokenSemicolon;
        RuleResult resultTokenComma;
        RuleResult resultTokenLAngle;
        RuleResult resultTokenRAngle;
        RuleResult resultTokenLAngleEqual;
        RuleResult resultTokenRAngleEqual;
        RuleResult resultTokenEqualEqual;
        RuleResult resultTokenEMarkEqual;
        RuleResult resultTokenEqualEqualEqual;
        RuleResult resultTokenEMarkEqualEqual;
        RuleResult resultTokenPlus;
        RuleResult resultTokenMinus;
        RuleResult resultTokenStar;
        RuleResult resultTokenStarStar;
        RuleResult resultTokenPercent;
        RuleResult resultTokenPlusPlus;
        RuleResult resultTokenMinusMinus;
        RuleResult resultTokenLAngleLAngle;
        RuleResult resultTokenRAngleRAngle;
        RuleResult resultTokenRAngleRAngleRAngle;
        RuleResult resultTokenAmp;
        RuleResult resultTokenPipe;
        RuleResult resultTokenCaret;
        RuleResult resultTokenEMark;
        RuleResult resultTokenTilde;
        RuleResult resultTokenAmpAmp;
        RuleResult resultTokenPipePipe;
        RuleResult resultTokenQMark;
        RuleResult resultTokenColon;
        RuleResult resultTokenEqual;
        RuleResult resultTokenPlusEqual;
        RuleResult resultTokenMinusEqual;
        RuleResult resultTokenStarEqual;
        RuleResult resultTokenPercentEqual;
        RuleResult resultTokenLAngleLAngleEqual;
        RuleResult resultTokenRAngleRAngleEqual;
        RuleResult resultTokenRAngleRAngleRAngleEqual;
        RuleResult resultTokenAmpEqual;
        RuleResult resultTokenPipeEqual;
        RuleResult resultTokenCaretEqual;
        RuleResult resultTokenArrow;
        RuleResult resultTokenFSlash;
        RuleResult resultTokenFSlashEqual;
        RuleResult resultTokenIdentifier[2][2];
        RuleResult resultTokenIdentifierOrYield[2][2][2];
        RuleResult resultTokenNumericLiteral;
        RuleResult resultTokenStringLiteral;
        RuleResult resultTokenRegularExpressionLiteral;
        RuleResult resultRegularExpressionLiteral;
        RuleResult resultIdentifierReference[2][2][2];
        RuleResult resultFunctionExpression[2][2];
        RuleResult resultGeneratorExpression[2][2];
        RuleResult resultClassExpression[2][2][2];
        RuleResult resultParenthesizedExpression[2][2][2];
        RuleResult resultPrimaryExpression[2][2][2];
        RuleResult resultLiteral;
        RuleResult resultArrayLiteralElement[2][2][2];
        RuleResult resultArrayLiteralElementList[2][2][2];
        RuleResult resultArrayLiteral[2][2][2];
        RuleResult resultObjectLiteral[2][2][2];
        RuleResult resultTemplateLiteralRest[2][2][2];
        RuleResult resultTemplateLiteral[2][2][2];
        RuleResult resultArrowFunction[2][2][2][2];
        RuleResult resultYieldExpression[2][2][2];
        RuleResult resultLeftHandSideExpression[2][2][2];
        RuleResult resultShiftExpression[2][2][2];
        RuleResult resultRelationalExpression[2][2][2][2];
        RuleResult resultEqualityExpression[2][2][2][2];
        RuleResult resultBitwiseANDExpression[2][2][2][2];
        RuleResult resultBitwiseXORExpression[2][2][2][2];
        RuleResult resultBitwiseORExpression[2][2][2][2];
        RuleResult resultLogicalANDExpression[2][2][2][2];
        RuleResult resultLogicalORExpression[2][2][2][2];
        RuleResult resultConditionalExpression[2][2][2][2];
        RuleResult resultAssignmentExpression[2][2][2][2];
        RuleResult resultExpression[2][2][2][2];
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
    template <bool lineTerminatorAllowed>
    void parseTokenizerComment();
    template <bool lineTerminatorAllowed>
    bool parseTokenizerMultiLineComment();
    void parseTokenizerSingleLineComment();
    template <bool lineTerminatorAllowed>
    void parseTokenizerTokenSeperator();
    void parseTokenizerTokenSeperatorWithLineTerminator();
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
    String parseIdentifierName();
    RawAndCookedString parseTokenNoSubstitutionTemplate();
    RawAndCookedString parseTokenTemplateHead();
    RawAndCookedString parseTokenTemplateMiddle();
    RawAndCookedString parseTokenTemplateTail();
    void parseTokenAwait();
    void parseTokenBreak();
    void parseTokenCase();
    void parseTokenCatch();
    void parseTokenClass();
    void parseTokenConst();
    void parseTokenContinue();
    void parseTokenDebugger();
    void parseTokenDefault();
    void parseTokenDelete();
    void parseTokenDo();
    void parseTokenElse();
    void parseTokenEnum();
    void parseTokenExport();
    void parseTokenExtends();
    void parseTokenFalse();
    void parseTokenFinally();
    void parseTokenFor();
    void parseTokenFunction();
    void parseTokenIf();
    void parseTokenImplements();
    void parseTokenImport();
    void parseTokenIn();
    void parseTokenInstanceOf();
    void parseTokenInterface();
    void parseTokenNew();
    void parseTokenNull();
    void parseTokenPackage();
    void parseTokenPrivate();
    void parseTokenProtected();
    void parseTokenPublic();
    void parseTokenReturn();
    void parseTokenSuper();
    void parseTokenSwitch();
    void parseTokenThis();
    void parseTokenThrow();
    void parseTokenTrue();
    void parseTokenTry();
    void parseTokenTypeOf();
    void parseTokenVar();
    void parseTokenVoid();
    void parseTokenWhile();
    void parseTokenWith();
    void parseTokenYield();
    void parseTokenLBrace();
    void parseTokenRBrace();
    void parseTokenLParen();
    void parseTokenRParen();
    void parseTokenLBracket();
    void parseTokenRBracket();
    void parseTokenPeriod();
    void parseTokenEllipsis();
    void parseTokenSemicolon();
    void parseTokenComma();
    void parseTokenLAngle();
    void parseTokenRAngle();
    void parseTokenLAngleEqual();
    void parseTokenRAngleEqual();
    void parseTokenEqualEqual();
    void parseTokenEMarkEqual();
    void parseTokenEqualEqualEqual();
    void parseTokenEMarkEqualEqual();
    void parseTokenPlus();
    void parseTokenMinus();
    void parseTokenStar();
    void parseTokenStarStar();
    void parseTokenPercent();
    void parseTokenPlusPlus();
    void parseTokenMinusMinus();
    void parseTokenLAngleLAngle();
    void parseTokenRAngleRAngle();
    void parseTokenRAngleRAngleRAngle();
    void parseTokenAmp();
    void parseTokenPipe();
    void parseTokenCaret();
    void parseTokenEMark();
    void parseTokenTilde();
    void parseTokenAmpAmp();
    void parseTokenPipePipe();
    void parseTokenQMark();
    void parseTokenColon();
    void parseTokenEqual();
    void parseTokenPlusEqual();
    void parseTokenMinusEqual();
    void parseTokenStarEqual();
    void parseTokenPercentEqual();
    void parseTokenLAngleLAngleEqual();
    void parseTokenRAngleRAngleEqual();
    void parseTokenRAngleRAngleRAngleEqual();
    void parseTokenAmpEqual();
    void parseTokenPipeEqual();
    void parseTokenCaretEqual();
    void parseTokenArrow();
    void parseTokenFSlash();
    void parseTokenFSlashEqual();
    template <bool isModule, bool isStrict>
    String parseTokenIdentifier();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    String parseTokenIdentifierOrYield();
    String parseTokenNumericLiteral();
    String parseTokenStringLiteral();
    BodyAndFlags parseTokenRegularExpressionLiteral();
    ExpressionPointer parseRegularExpressionLiteral();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseIdentifierReference();
    template <bool isModule, bool isStrict>
    ExpressionPointer parseFunctionExpression();
    template <bool isModule, bool isStrict>
    ExpressionPointer parseGeneratorExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseClassExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseParenthesizedExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parsePrimaryExpression();
    ExpressionPointer parseLiteral();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ArrayLiteralElementPointer parseArrayLiteralElement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ArrayLiteralElementList parseArrayLiteralElementList();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseArrayLiteral();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseObjectLiteral();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::TemplateRest parseTemplateLiteralRest();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseTemplateLiteral();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseArrowFunction();
    template <bool isModule, bool isStrict, bool canHaveInOperator>
    ExpressionPointer parseYieldExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseLeftHandSideExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseShiftExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseRelationalExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseEqualityExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseBitwiseANDExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseBitwiseXORExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseBitwiseORExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseLogicalANDExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseLogicalORExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseConditionalExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseAssignmentExpression();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseExpression();

private:
    unsigned internalParseTokenizerHexDigitValue(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    char32_t internalParseTokenizerLineTerminatorSequence(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool lineTerminatorAllowed>
    void internalParseTokenizerComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool lineTerminatorAllowed>
    bool internalParseTokenizerMultiLineComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerSingleLineComment(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool lineTerminatorAllowed>
    void internalParseTokenizerTokenSeperator(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerTokenSeperatorWithLineTerminator(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
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
    String internalParseIdentifierName(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenNoSubstitutionTemplate(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenTemplateHead(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenTemplateMiddle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    RawAndCookedString internalParseTokenTemplateTail(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenAwait(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenBreak(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenCase(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenCatch(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenClass(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenConst(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenContinue(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenDebugger(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenDefault(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenDelete(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenDo(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenElse(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenEnum(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenExport(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenExtends(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenFalse(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenFinally(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenFor(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenFunction(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenIf(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenImplements(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenImport(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenIn(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenInstanceOf(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenInterface(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenNew(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenNull(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPackage(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPrivate(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenProtected(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPublic(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenReturn(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenSuper(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenSwitch(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenThis(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenThrow(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenTrue(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenTry(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenTypeOf(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenVar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenVoid(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenWhile(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenWith(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenYield(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenLBrace(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRBrace(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenLParen(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRParen(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenLBracket(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRBracket(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPeriod(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenEllipsis(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenSemicolon(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenComma(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenLAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenLAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenEqualEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenEMarkEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenEqualEqualEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenEMarkEqualEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPlus(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenMinus(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenStar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenStarStar(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPercent(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPlusPlus(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenMinusMinus(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenLAngleLAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRAngleRAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRAngleRAngleRAngle(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenAmp(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPipe(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenCaret(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenEMark(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenTilde(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenAmpAmp(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPipePipe(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenQMark(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenColon(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPlusEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenMinusEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenStarEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPercentEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenLAngleLAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRAngleRAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenRAngleRAngleRAngleEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenAmpEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPipeEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenCaretEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenArrow(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenFSlash(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenFSlashEqual(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict>
    String internalParseTokenIdentifier(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    String internalParseTokenIdentifierOrYield(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenNumericLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenStringLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    BodyAndFlags internalParseTokenRegularExpressionLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    ExpressionPointer internalParseRegularExpressionLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseIdentifierReference(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict>
    ExpressionPointer internalParseFunctionExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict>
    ExpressionPointer internalParseGeneratorExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseClassExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseParenthesizedExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParsePrimaryExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    ExpressionPointer internalParseLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ArrayLiteralElementPointer internalParseArrayLiteralElement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ArrayLiteralElementList internalParseArrayLiteralElementList(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseArrayLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseObjectLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::TemplateRest internalParseTemplateLiteralRest(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseTemplateLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseArrowFunction(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator>
    ExpressionPointer internalParseYieldExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseLeftHandSideExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseShiftExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseRelationalExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseEqualityExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseBitwiseANDExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseBitwiseXORExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseBitwiseORExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseLogicalANDExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseLogicalORExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseConditionalExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseAssignmentExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
};

extern template void Parser::parseTokenizerComment<false>();
extern template void Parser::internalParseTokenizerComment<false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerComment<true>();
extern template void Parser::internalParseTokenizerComment<true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template bool Parser::parseTokenizerMultiLineComment<false>();
extern template bool Parser::internalParseTokenizerMultiLineComment<false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template bool Parser::parseTokenizerMultiLineComment<true>();
extern template bool Parser::internalParseTokenizerMultiLineComment<true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerTokenSeperator<false>();
extern template void Parser::internalParseTokenizerTokenSeperator<false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template void Parser::parseTokenizerTokenSeperator<true>();
extern template void Parser::internalParseTokenizerTokenSeperator<true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
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
extern template String Parser::parseTokenIdentifier<false, false>();
extern template String Parser::internalParseTokenIdentifier<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifier<false, true>();
extern template String Parser::internalParseTokenIdentifier<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifier<true, false>();
extern template String Parser::internalParseTokenIdentifier<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifier<true, true>();
extern template String Parser::internalParseTokenIdentifier<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifierOrYield<false, false, false>();
extern template String Parser::internalParseTokenIdentifierOrYield<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifierOrYield<false, false, true>();
extern template String Parser::internalParseTokenIdentifierOrYield<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifierOrYield<false, true, false>();
extern template String Parser::internalParseTokenIdentifierOrYield<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifierOrYield<false, true, true>();
extern template String Parser::internalParseTokenIdentifierOrYield<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifierOrYield<true, false, false>();
extern template String Parser::internalParseTokenIdentifierOrYield<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifierOrYield<true, false, true>();
extern template String Parser::internalParseTokenIdentifierOrYield<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifierOrYield<true, true, false>();
extern template String Parser::internalParseTokenIdentifierOrYield<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseTokenIdentifierOrYield<true, true, true>();
extern template String Parser::internalParseTokenIdentifierOrYield<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseIdentifierReference<false, false, false>();
extern template ExpressionPointer Parser::internalParseIdentifierReference<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseIdentifierReference<false, false, true>();
extern template ExpressionPointer Parser::internalParseIdentifierReference<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseIdentifierReference<false, true, false>();
extern template ExpressionPointer Parser::internalParseIdentifierReference<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseIdentifierReference<false, true, true>();
extern template ExpressionPointer Parser::internalParseIdentifierReference<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseIdentifierReference<true, false, false>();
extern template ExpressionPointer Parser::internalParseIdentifierReference<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseIdentifierReference<true, false, true>();
extern template ExpressionPointer Parser::internalParseIdentifierReference<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseIdentifierReference<true, true, false>();
extern template ExpressionPointer Parser::internalParseIdentifierReference<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseIdentifierReference<true, true, true>();
extern template ExpressionPointer Parser::internalParseIdentifierReference<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseFunctionExpression<false, false>();
extern template ExpressionPointer Parser::internalParseFunctionExpression<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseFunctionExpression<false, true>();
extern template ExpressionPointer Parser::internalParseFunctionExpression<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseFunctionExpression<true, false>();
extern template ExpressionPointer Parser::internalParseFunctionExpression<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseFunctionExpression<true, true>();
extern template ExpressionPointer Parser::internalParseFunctionExpression<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseGeneratorExpression<false, false>();
extern template ExpressionPointer Parser::internalParseGeneratorExpression<false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseGeneratorExpression<false, true>();
extern template ExpressionPointer Parser::internalParseGeneratorExpression<false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseGeneratorExpression<true, false>();
extern template ExpressionPointer Parser::internalParseGeneratorExpression<true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseGeneratorExpression<true, true>();
extern template ExpressionPointer Parser::internalParseGeneratorExpression<true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseClassExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseClassExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseClassExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseClassExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseClassExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseClassExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseClassExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseClassExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseClassExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseClassExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseClassExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseClassExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseClassExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseClassExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseClassExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseClassExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseParenthesizedExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseParenthesizedExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseParenthesizedExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseParenthesizedExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseParenthesizedExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseParenthesizedExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseParenthesizedExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseParenthesizedExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseParenthesizedExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseParenthesizedExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseParenthesizedExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseParenthesizedExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseParenthesizedExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseParenthesizedExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseParenthesizedExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseParenthesizedExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePrimaryExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParsePrimaryExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePrimaryExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParsePrimaryExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePrimaryExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParsePrimaryExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePrimaryExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParsePrimaryExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePrimaryExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParsePrimaryExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePrimaryExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParsePrimaryExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePrimaryExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParsePrimaryExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePrimaryExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParsePrimaryExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<false, false, false>();
extern template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<false, false, true>();
extern template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<false, true, false>();
extern template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<false, true, true>();
extern template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<true, false, false>();
extern template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<true, false, true>();
extern template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<true, true, false>();
extern template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementPointer Parser::parseArrayLiteralElement<true, true, true>();
extern template ArrayLiteralElementPointer Parser::internalParseArrayLiteralElement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementList Parser::parseArrayLiteralElementList<false, false, false>();
extern template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementList Parser::parseArrayLiteralElementList<false, false, true>();
extern template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementList Parser::parseArrayLiteralElementList<false, true, false>();
extern template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementList Parser::parseArrayLiteralElementList<false, true, true>();
extern template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementList Parser::parseArrayLiteralElementList<true, false, false>();
extern template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementList Parser::parseArrayLiteralElementList<true, false, true>();
extern template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementList Parser::parseArrayLiteralElementList<true, true, false>();
extern template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArrayLiteralElementList Parser::parseArrayLiteralElementList<true, true, true>();
extern template ArrayLiteralElementList Parser::internalParseArrayLiteralElementList<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrayLiteral<false, false, false>();
extern template ExpressionPointer Parser::internalParseArrayLiteral<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrayLiteral<false, false, true>();
extern template ExpressionPointer Parser::internalParseArrayLiteral<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrayLiteral<false, true, false>();
extern template ExpressionPointer Parser::internalParseArrayLiteral<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrayLiteral<false, true, true>();
extern template ExpressionPointer Parser::internalParseArrayLiteral<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrayLiteral<true, false, false>();
extern template ExpressionPointer Parser::internalParseArrayLiteral<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrayLiteral<true, false, true>();
extern template ExpressionPointer Parser::internalParseArrayLiteral<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrayLiteral<true, true, false>();
extern template ExpressionPointer Parser::internalParseArrayLiteral<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrayLiteral<true, true, true>();
extern template ExpressionPointer Parser::internalParseArrayLiteral<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseObjectLiteral<false, false, false>();
extern template ExpressionPointer Parser::internalParseObjectLiteral<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseObjectLiteral<false, false, true>();
extern template ExpressionPointer Parser::internalParseObjectLiteral<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseObjectLiteral<false, true, false>();
extern template ExpressionPointer Parser::internalParseObjectLiteral<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseObjectLiteral<false, true, true>();
extern template ExpressionPointer Parser::internalParseObjectLiteral<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseObjectLiteral<true, false, false>();
extern template ExpressionPointer Parser::internalParseObjectLiteral<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseObjectLiteral<true, false, true>();
extern template ExpressionPointer Parser::internalParseObjectLiteral<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseObjectLiteral<true, true, false>();
extern template ExpressionPointer Parser::internalParseObjectLiteral<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseObjectLiteral<true, true, true>();
extern template ExpressionPointer Parser::internalParseObjectLiteral<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::TemplateRest Parser::parseTemplateLiteralRest<false, false, false>();
extern template ast::TemplateRest Parser::internalParseTemplateLiteralRest<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::TemplateRest Parser::parseTemplateLiteralRest<false, false, true>();
extern template ast::TemplateRest Parser::internalParseTemplateLiteralRest<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::TemplateRest Parser::parseTemplateLiteralRest<false, true, false>();
extern template ast::TemplateRest Parser::internalParseTemplateLiteralRest<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::TemplateRest Parser::parseTemplateLiteralRest<false, true, true>();
extern template ast::TemplateRest Parser::internalParseTemplateLiteralRest<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::TemplateRest Parser::parseTemplateLiteralRest<true, false, false>();
extern template ast::TemplateRest Parser::internalParseTemplateLiteralRest<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::TemplateRest Parser::parseTemplateLiteralRest<true, false, true>();
extern template ast::TemplateRest Parser::internalParseTemplateLiteralRest<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::TemplateRest Parser::parseTemplateLiteralRest<true, true, false>();
extern template ast::TemplateRest Parser::internalParseTemplateLiteralRest<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::TemplateRest Parser::parseTemplateLiteralRest<true, true, true>();
extern template ast::TemplateRest Parser::internalParseTemplateLiteralRest<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseTemplateLiteral<false, false, false>();
extern template ExpressionPointer Parser::internalParseTemplateLiteral<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseTemplateLiteral<false, false, true>();
extern template ExpressionPointer Parser::internalParseTemplateLiteral<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseTemplateLiteral<false, true, false>();
extern template ExpressionPointer Parser::internalParseTemplateLiteral<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseTemplateLiteral<false, true, true>();
extern template ExpressionPointer Parser::internalParseTemplateLiteral<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseTemplateLiteral<true, false, false>();
extern template ExpressionPointer Parser::internalParseTemplateLiteral<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseTemplateLiteral<true, false, true>();
extern template ExpressionPointer Parser::internalParseTemplateLiteral<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseTemplateLiteral<true, true, false>();
extern template ExpressionPointer Parser::internalParseTemplateLiteral<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseTemplateLiteral<true, true, true>();
extern template ExpressionPointer Parser::internalParseTemplateLiteral<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseArrowFunction<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseArrowFunction<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseArrowFunction<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseArrowFunction<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseArrowFunction<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseArrowFunction<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseArrowFunction<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseArrowFunction<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseArrowFunction<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseArrowFunction<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseArrowFunction<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseArrowFunction<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseArrowFunction<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseArrowFunction<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseArrowFunction<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseArrowFunction<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseArrowFunction<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseYieldExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseYieldExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseYieldExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseYieldExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseYieldExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseYieldExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseYieldExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseYieldExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseYieldExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseYieldExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseYieldExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseYieldExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseYieldExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseYieldExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseYieldExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseYieldExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLeftHandSideExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseLeftHandSideExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLeftHandSideExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseLeftHandSideExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLeftHandSideExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseLeftHandSideExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLeftHandSideExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseLeftHandSideExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLeftHandSideExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseLeftHandSideExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLeftHandSideExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseLeftHandSideExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLeftHandSideExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseLeftHandSideExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLeftHandSideExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseLeftHandSideExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseShiftExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseShiftExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseShiftExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseShiftExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseShiftExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseShiftExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseShiftExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseShiftExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseShiftExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseShiftExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseShiftExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseShiftExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseShiftExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseShiftExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseShiftExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseShiftExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseRelationalExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseRelationalExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseEqualityExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseEqualityExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseANDExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseANDExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseXORExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseXORExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseBitwiseORExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseBitwiseORExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalANDExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseLogicalANDExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseLogicalORExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseLogicalORExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseConditionalExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseConditionalExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAssignmentExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseAssignmentExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseExpression<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseExpression<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseExpression<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseExpression<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseExpression<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseExpression<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseExpression<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseExpression<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseExpression<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseExpression<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseExpression<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseExpression<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseExpression<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseExpression<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseExpression<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseExpression<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseExpression<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_ */
