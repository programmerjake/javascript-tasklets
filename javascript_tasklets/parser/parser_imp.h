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

// to regenerate parser_imp.h and parser_imp.cpp, use https://github.com/programmerjake/peg_parser_generator
// command: peg_parser_generator javascript_tasklets/parser/parser_imp.peg

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
#include "ast/statement.h"
#include "ast/declaration.h"
#include "ast/literal.h"
#include "ast/arena.h"
#include "ast/argument.h"
#include "ast/binding.h"

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

struct DirectivePrologue final
{
    bool useStrict = false;
};

typedef ast::Node *NodePointer;
typedef ast::Expression *ExpressionPointer;
typedef ast::ArrayLiteralElement *ArrayLiteralElementPointer;
typedef std::vector<ast::ArrayLiteralElement *> ArrayLiteralElementList;
typedef ast::Argument *ArgumentPointer;
typedef ast::ExpressionTemplateLiteral *ExpressionTemplateLiteralPointer;
typedef ast::Statement *StatementPointer;
typedef ast::Binding *BindingPointer;
typedef ast::PropertyName *PropertyNamePointer;
}
}

#line 116 "javascript_tasklets/parser/parser_imp.h"

namespace javascript_tasklets
{
namespace parser
{
class Parser final
{
    Parser(const Parser &) = delete;
    Parser &operator=(const Parser &) = delete;
#line 103 "javascript_tasklets/parser/parser_imp.peg"
            
public:
    ast::Arena arena = ast::Arena();

#line 131 "javascript_tasklets/parser/parser_imp.h"

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
        RuleResult resultTokenizerLet;
        RuleResult resultTokenizerNew;
        RuleResult resultTokenizerNull;
        RuleResult resultTokenizerPackage;
        RuleResult resultTokenizerPrivate;
        RuleResult resultTokenizerProtected;
        RuleResult resultTokenizerPublic;
        RuleResult resultTokenizerReturn;
        RuleResult resultTokenizerSuper;
        RuleResult resultTokenizerSwitch;
        RuleResult resultTokenizerTarget;
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
        RuleResult resultTokenLet;
        RuleResult resultTokenNew;
        RuleResult resultTokenNull;
        RuleResult resultTokenPackage;
        RuleResult resultTokenPrivate;
        RuleResult resultTokenProtected;
        RuleResult resultTokenPublic;
        RuleResult resultTokenReturn;
        RuleResult resultTokenSuper;
        RuleResult resultTokenSwitch;
        RuleResult resultTokenTarget;
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
        RuleResult resultTokenRequiredSemicolon;
        RuleResult resultTokenSemicolon;
        RuleResult resultTokenIdentifierName;
        RuleResult resultTokenIdentifier[2][2];
        RuleResult resultTokenIdentifierOrYield[2][2][2];
        RuleResult resultTokenLocation;
        RuleResult resultTokenNumericLiteral;
        RuleResult resultTokenStringLiteral;
        RuleResult resultTokenRegularExpressionLiteral;
        RuleResult resultRegularExpressionLiteral;
        RuleResult resultDirectivePrologue;
        RuleResult resultIdentifierReference[2][2][2];
        RuleResult resultBindingIdentifier[2][2][2];
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
        RuleResult resultPropertyName[2][2][2];
        RuleResult resultInitializer[2][2][2][2];
        RuleResult resultTemplateLiteral[2][2][2];
        RuleResult resultTemplateLiteralRest[2][2][2];
        RuleResult resultArrowFunction[2][2][2][2];
        RuleResult resultYieldExpression[2][2][2];
        RuleResult resultMemberExpression[2][2][2];
        RuleResult resultSuperProperty[2][2][2];
        RuleResult resultMetaProperty;
        RuleResult resultNewTarget;
        RuleResult resultNewExpression[2][2][2];
        RuleResult resultCallExpression[2][2][2];
        RuleResult resultSuperCall[2][2][2];
        RuleResult resultArguments[2][2][2];
        RuleResult resultArgument[2][2][2];
        RuleResult resultArgumentList[2][2][2];
        RuleResult resultLeftHandSideExpression[2][2][2];
        RuleResult resultPostfixExpression[2][2][2];
        RuleResult resultUnaryExpression[2][2][2];
        RuleResult resultMultiplicativeExpression[2][2][2];
        RuleResult resultAdditiveExpression[2][2][2];
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
        RuleResult resultContinueStatement[2][2][2];
        RuleResult resultBreakStatement[2][2][2];
        RuleResult resultReturnStatement[2][2][2];
        RuleResult resultWithStatement[2][2][2][2];
        RuleResult resultLabelledStatement[2][2][2][2];
        RuleResult resultThrowStatement[2][2][2];
        RuleResult resultTryStatement[2][2][2][2];
        RuleResult resultDebuggerStatement;
        RuleResult resultStatement[2][2][2][2];
        RuleResult resultDeclaration[2][2][2];
        RuleResult resultHoistableDeclaration[2][2][2][2];
        RuleResult resultFunctionDeclaration[2][2][2][2];
        RuleResult resultGeneratorDeclaration[2][2][2][2];
        RuleResult resultClassDeclaration[2][2][2][2];
        RuleResult resultSwitchStatement[2][2][2][2];
        RuleResult resultBreakableStatement[2][2][2][2];
        RuleResult resultBlockStatement[2][2][2][2];
        RuleResult resultBlock[2][2][2][2];
        RuleResult resultStatementList[2][2][2][2];
        RuleResult resultStatementListItem[2][2][2][2];
        RuleResult resultLexicalDeclaration[2][2][2][2][2];
        RuleResult resultBindingList[2][2][2][2];
        RuleResult resultLexicalBinding[2][2][2][2];
        RuleResult resultVariableStatement[2][2][2];
        RuleResult resultVariableDeclarationList[2][2][2][2];
        RuleResult resultVariableDeclaration[2][2][2][2];
        RuleResult resultBindingPattern[2][2][2];
        RuleResult resultObjectBindingPattern[2][2][2];
        RuleResult resultArrayBindingPattern[2][2][2];
        RuleResult resultArrayBindingElementList[2][2][2];
        RuleResult resultBindingPropertyList[2][2][2];
        RuleResult resultBindingProperty[2][2][2];
        RuleResult resultBindingElement[2][2][2];
        RuleResult resultBindingRestElement[2][2][2];
        RuleResult resultSingleNameBinding[2][2][2];
        RuleResult resultEmptyStatement;
        RuleResult resultExpressionStatement[2][2][2];
        RuleResult resultIfStatement[2][2][2][2];
        RuleResult resultIterationStatement[2][2][2][2];
        RuleResult resultScript;
        RuleResult resultScriptBody;
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
    void parseTokenizerLet();
    void parseTokenizerNew();
    void parseTokenizerNull();
    void parseTokenizerPackage();
    void parseTokenizerPrivate();
    void parseTokenizerProtected();
    void parseTokenizerPublic();
    void parseTokenizerReturn();
    void parseTokenizerSuper();
    void parseTokenizerSwitch();
    void parseTokenizerTarget();
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
    void parseTokenLet();
    void parseTokenNew();
    void parseTokenNull();
    void parseTokenPackage();
    void parseTokenPrivate();
    void parseTokenProtected();
    void parseTokenPublic();
    void parseTokenReturn();
    void parseTokenSuper();
    void parseTokenSwitch();
    void parseTokenTarget();
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
    std::size_t parseTokenRequiredSemicolon();
    std::size_t parseTokenSemicolon();
    void parseTokenSemicolonAfterDoWhile();
    String parseTokenIdentifierName();
    template <bool isModule, bool isStrict>
    String parseTokenIdentifier();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    String parseTokenIdentifierOrYield();
    std::size_t parseTokenLocation();
    String parseTokenNumericLiteral();
    String parseTokenStringLiteral();
    BodyAndFlags parseTokenRegularExpressionLiteral();
    ExpressionPointer parseRegularExpressionLiteral();
    DirectivePrologue parseDirectivePrologue();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseIdentifierReference();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    String parseBindingIdentifier();
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
    PropertyNamePointer parsePropertyName();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseInitializer();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionTemplateLiteralPointer parseTemplateLiteral();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::TemplateRest parseTemplateLiteralRest();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer parseArrowFunction();
    template <bool isModule, bool isStrict, bool canHaveInOperator>
    ExpressionPointer parseYieldExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseMemberExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseSuperProperty();
    ExpressionPointer parseMetaProperty();
    ExpressionPointer parseNewTarget();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseNewExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseCallExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseSuperCall();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::ArgumentList parseArguments();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ArgumentPointer parseArgument();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::ArgumentList parseArgumentList();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseLeftHandSideExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parsePostfixExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseUnaryExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseMultiplicativeExpression();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer parseAdditiveExpression();
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
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer parseContinueStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer parseBreakStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer parseReturnStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseWithStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseLabelledStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer parseThrowStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseTryStatement();
    StatementPointer parseDebuggerStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer parseDeclaration();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveEmptyName>
    StatementPointer parseHoistableDeclaration();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveEmptyName>
    StatementPointer parseFunctionDeclaration();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveEmptyName>
    StatementPointer parseGeneratorDeclaration();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveEmptyName>
    StatementPointer parseClassDeclaration();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseSwitchStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseBreakableStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseBlockStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseBlock();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    ast::StatementList parseStatementList();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseStatementListItem();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator, bool isSemicolonRequired>
    StatementPointer parseLexicalDeclaration();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ast::BindingList parseBindingList();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    BindingPointer parseLexicalBinding();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer parseVariableStatement();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ast::BindingList parseVariableDeclarationList();
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    BindingPointer parseVariableDeclaration();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer parseBindingPattern();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer parseObjectBindingPattern();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer parseArrayBindingPattern();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::BindingList parseArrayBindingElementList();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::BindingList parseBindingPropertyList();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer parseBindingProperty();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer parseBindingElement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer parseBindingRestElement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer parseSingleNameBinding();
    StatementPointer parseEmptyStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer parseExpressionStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseIfStatement();
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer parseIterationStatement();
    ast::StatementList parseScript();
    ast::StatementList parseScriptBody();

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
    void internalParseTokenizerLet(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerNew(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerNull(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPackage(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPrivate(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerProtected(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerPublic(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerReturn(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerSuper(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerSwitch(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenizerTarget(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
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
    void internalParseTokenLet(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenNew(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenNull(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPackage(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPrivate(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenProtected(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenPublic(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenReturn(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenSuper(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenSwitch(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenTarget(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
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
    std::size_t internalParseTokenRequiredSemicolon(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    std::size_t internalParseTokenSemicolon(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    void internalParseTokenSemicolonAfterDoWhile(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenIdentifierName(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict>
    String internalParseTokenIdentifier(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    String internalParseTokenIdentifierOrYield(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    std::size_t internalParseTokenLocation(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenNumericLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    String internalParseTokenStringLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    BodyAndFlags internalParseTokenRegularExpressionLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    ExpressionPointer internalParseRegularExpressionLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    DirectivePrologue internalParseDirectivePrologue(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseIdentifierReference(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    String internalParseBindingIdentifier(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
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
    PropertyNamePointer internalParsePropertyName(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseInitializer(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionTemplateLiteralPointer internalParseTemplateLiteral(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::TemplateRest internalParseTemplateLiteralRest(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ExpressionPointer internalParseArrowFunction(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator>
    ExpressionPointer internalParseYieldExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseMemberExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseSuperProperty(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    ExpressionPointer internalParseMetaProperty(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    ExpressionPointer internalParseNewTarget(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseNewExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseCallExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseSuperCall(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::ArgumentList internalParseArguments(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ArgumentPointer internalParseArgument(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::ArgumentList internalParseArgumentList(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseLeftHandSideExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParsePostfixExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseUnaryExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseMultiplicativeExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ExpressionPointer internalParseAdditiveExpression(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
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
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer internalParseContinueStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer internalParseBreakStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer internalParseReturnStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseWithStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseLabelledStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer internalParseThrowStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseTryStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    StatementPointer internalParseDebuggerStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer internalParseDeclaration(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveEmptyName>
    StatementPointer internalParseHoistableDeclaration(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveEmptyName>
    StatementPointer internalParseFunctionDeclaration(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveEmptyName>
    StatementPointer internalParseGeneratorDeclaration(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveEmptyName>
    StatementPointer internalParseClassDeclaration(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseSwitchStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseBreakableStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseBlockStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseBlock(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    ast::StatementList internalParseStatementList(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseStatementListItem(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator, bool isSemicolonRequired>
    StatementPointer internalParseLexicalDeclaration(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ast::BindingList internalParseBindingList(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    BindingPointer internalParseLexicalBinding(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer internalParseVariableStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    ast::BindingList internalParseVariableDeclarationList(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveInOperator, bool canHaveYieldOperator>
    BindingPointer internalParseVariableDeclaration(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer internalParseBindingPattern(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer internalParseObjectBindingPattern(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer internalParseArrayBindingPattern(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::BindingList internalParseArrayBindingElementList(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    ast::BindingList internalParseBindingPropertyList(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer internalParseBindingProperty(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer internalParseBindingElement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer internalParseBindingRestElement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    BindingPointer internalParseSingleNameBinding(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    StatementPointer internalParseEmptyStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator>
    StatementPointer internalParseExpressionStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseIfStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    template <bool isModule, bool isStrict, bool canHaveYieldOperator, bool canHaveReturnStatement>
    StatementPointer internalParseIterationStatement(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    ast::StatementList internalParseScript(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
    ast::StatementList internalParseScriptBody(std::size_t startLocation, RuleResult &ruleResult, bool isRequiredForSuccess);
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
extern template String Parser::parseBindingIdentifier<false, false, false>();
extern template String Parser::internalParseBindingIdentifier<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseBindingIdentifier<false, false, true>();
extern template String Parser::internalParseBindingIdentifier<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseBindingIdentifier<false, true, false>();
extern template String Parser::internalParseBindingIdentifier<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseBindingIdentifier<false, true, true>();
extern template String Parser::internalParseBindingIdentifier<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseBindingIdentifier<true, false, false>();
extern template String Parser::internalParseBindingIdentifier<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseBindingIdentifier<true, false, true>();
extern template String Parser::internalParseBindingIdentifier<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseBindingIdentifier<true, true, false>();
extern template String Parser::internalParseBindingIdentifier<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template String Parser::parseBindingIdentifier<true, true, true>();
extern template String Parser::internalParseBindingIdentifier<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
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
extern template PropertyNamePointer Parser::parsePropertyName<false, false, false>();
extern template PropertyNamePointer Parser::internalParsePropertyName<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template PropertyNamePointer Parser::parsePropertyName<false, false, true>();
extern template PropertyNamePointer Parser::internalParsePropertyName<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template PropertyNamePointer Parser::parsePropertyName<false, true, false>();
extern template PropertyNamePointer Parser::internalParsePropertyName<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template PropertyNamePointer Parser::parsePropertyName<false, true, true>();
extern template PropertyNamePointer Parser::internalParsePropertyName<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template PropertyNamePointer Parser::parsePropertyName<true, false, false>();
extern template PropertyNamePointer Parser::internalParsePropertyName<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template PropertyNamePointer Parser::parsePropertyName<true, false, true>();
extern template PropertyNamePointer Parser::internalParsePropertyName<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template PropertyNamePointer Parser::parsePropertyName<true, true, false>();
extern template PropertyNamePointer Parser::internalParsePropertyName<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template PropertyNamePointer Parser::parsePropertyName<true, true, true>();
extern template PropertyNamePointer Parser::internalParsePropertyName<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<false, false, false, false>();
extern template ExpressionPointer Parser::internalParseInitializer<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<false, false, false, true>();
extern template ExpressionPointer Parser::internalParseInitializer<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<false, false, true, false>();
extern template ExpressionPointer Parser::internalParseInitializer<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<false, false, true, true>();
extern template ExpressionPointer Parser::internalParseInitializer<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<false, true, false, false>();
extern template ExpressionPointer Parser::internalParseInitializer<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<false, true, false, true>();
extern template ExpressionPointer Parser::internalParseInitializer<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<false, true, true, false>();
extern template ExpressionPointer Parser::internalParseInitializer<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<false, true, true, true>();
extern template ExpressionPointer Parser::internalParseInitializer<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<true, false, false, false>();
extern template ExpressionPointer Parser::internalParseInitializer<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<true, false, false, true>();
extern template ExpressionPointer Parser::internalParseInitializer<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<true, false, true, false>();
extern template ExpressionPointer Parser::internalParseInitializer<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<true, false, true, true>();
extern template ExpressionPointer Parser::internalParseInitializer<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<true, true, false, false>();
extern template ExpressionPointer Parser::internalParseInitializer<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<true, true, false, true>();
extern template ExpressionPointer Parser::internalParseInitializer<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<true, true, true, false>();
extern template ExpressionPointer Parser::internalParseInitializer<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseInitializer<true, true, true, true>();
extern template ExpressionPointer Parser::internalParseInitializer<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionTemplateLiteralPointer Parser::parseTemplateLiteral<false, false, false>();
extern template ExpressionTemplateLiteralPointer Parser::internalParseTemplateLiteral<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionTemplateLiteralPointer Parser::parseTemplateLiteral<false, false, true>();
extern template ExpressionTemplateLiteralPointer Parser::internalParseTemplateLiteral<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionTemplateLiteralPointer Parser::parseTemplateLiteral<false, true, false>();
extern template ExpressionTemplateLiteralPointer Parser::internalParseTemplateLiteral<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionTemplateLiteralPointer Parser::parseTemplateLiteral<false, true, true>();
extern template ExpressionTemplateLiteralPointer Parser::internalParseTemplateLiteral<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionTemplateLiteralPointer Parser::parseTemplateLiteral<true, false, false>();
extern template ExpressionTemplateLiteralPointer Parser::internalParseTemplateLiteral<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionTemplateLiteralPointer Parser::parseTemplateLiteral<true, false, true>();
extern template ExpressionTemplateLiteralPointer Parser::internalParseTemplateLiteral<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionTemplateLiteralPointer Parser::parseTemplateLiteral<true, true, false>();
extern template ExpressionTemplateLiteralPointer Parser::internalParseTemplateLiteral<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionTemplateLiteralPointer Parser::parseTemplateLiteral<true, true, true>();
extern template ExpressionTemplateLiteralPointer Parser::internalParseTemplateLiteral<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
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
extern template ExpressionPointer Parser::parseMemberExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseMemberExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMemberExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseMemberExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMemberExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseMemberExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMemberExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseMemberExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMemberExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseMemberExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMemberExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseMemberExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMemberExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseMemberExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMemberExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseMemberExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperProperty<false, false, false>();
extern template ExpressionPointer Parser::internalParseSuperProperty<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperProperty<false, false, true>();
extern template ExpressionPointer Parser::internalParseSuperProperty<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperProperty<false, true, false>();
extern template ExpressionPointer Parser::internalParseSuperProperty<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperProperty<false, true, true>();
extern template ExpressionPointer Parser::internalParseSuperProperty<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperProperty<true, false, false>();
extern template ExpressionPointer Parser::internalParseSuperProperty<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperProperty<true, false, true>();
extern template ExpressionPointer Parser::internalParseSuperProperty<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperProperty<true, true, false>();
extern template ExpressionPointer Parser::internalParseSuperProperty<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperProperty<true, true, true>();
extern template ExpressionPointer Parser::internalParseSuperProperty<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseNewExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseNewExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseNewExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseNewExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseNewExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseNewExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseNewExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseNewExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseNewExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseNewExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseNewExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseNewExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseNewExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseNewExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseNewExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseNewExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseCallExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseCallExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseCallExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseCallExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseCallExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseCallExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseCallExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseCallExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseCallExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseCallExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseCallExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseCallExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseCallExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseCallExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseCallExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseCallExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperCall<false, false, false>();
extern template ExpressionPointer Parser::internalParseSuperCall<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperCall<false, false, true>();
extern template ExpressionPointer Parser::internalParseSuperCall<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperCall<false, true, false>();
extern template ExpressionPointer Parser::internalParseSuperCall<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperCall<false, true, true>();
extern template ExpressionPointer Parser::internalParseSuperCall<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperCall<true, false, false>();
extern template ExpressionPointer Parser::internalParseSuperCall<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperCall<true, false, true>();
extern template ExpressionPointer Parser::internalParseSuperCall<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperCall<true, true, false>();
extern template ExpressionPointer Parser::internalParseSuperCall<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseSuperCall<true, true, true>();
extern template ExpressionPointer Parser::internalParseSuperCall<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArguments<false, false, false>();
extern template ast::ArgumentList Parser::internalParseArguments<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArguments<false, false, true>();
extern template ast::ArgumentList Parser::internalParseArguments<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArguments<false, true, false>();
extern template ast::ArgumentList Parser::internalParseArguments<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArguments<false, true, true>();
extern template ast::ArgumentList Parser::internalParseArguments<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArguments<true, false, false>();
extern template ast::ArgumentList Parser::internalParseArguments<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArguments<true, false, true>();
extern template ast::ArgumentList Parser::internalParseArguments<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArguments<true, true, false>();
extern template ast::ArgumentList Parser::internalParseArguments<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArguments<true, true, true>();
extern template ast::ArgumentList Parser::internalParseArguments<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArgumentPointer Parser::parseArgument<false, false, false>();
extern template ArgumentPointer Parser::internalParseArgument<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArgumentPointer Parser::parseArgument<false, false, true>();
extern template ArgumentPointer Parser::internalParseArgument<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArgumentPointer Parser::parseArgument<false, true, false>();
extern template ArgumentPointer Parser::internalParseArgument<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArgumentPointer Parser::parseArgument<false, true, true>();
extern template ArgumentPointer Parser::internalParseArgument<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArgumentPointer Parser::parseArgument<true, false, false>();
extern template ArgumentPointer Parser::internalParseArgument<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArgumentPointer Parser::parseArgument<true, false, true>();
extern template ArgumentPointer Parser::internalParseArgument<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArgumentPointer Parser::parseArgument<true, true, false>();
extern template ArgumentPointer Parser::internalParseArgument<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ArgumentPointer Parser::parseArgument<true, true, true>();
extern template ArgumentPointer Parser::internalParseArgument<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArgumentList<false, false, false>();
extern template ast::ArgumentList Parser::internalParseArgumentList<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArgumentList<false, false, true>();
extern template ast::ArgumentList Parser::internalParseArgumentList<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArgumentList<false, true, false>();
extern template ast::ArgumentList Parser::internalParseArgumentList<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArgumentList<false, true, true>();
extern template ast::ArgumentList Parser::internalParseArgumentList<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArgumentList<true, false, false>();
extern template ast::ArgumentList Parser::internalParseArgumentList<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArgumentList<true, false, true>();
extern template ast::ArgumentList Parser::internalParseArgumentList<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArgumentList<true, true, false>();
extern template ast::ArgumentList Parser::internalParseArgumentList<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::ArgumentList Parser::parseArgumentList<true, true, true>();
extern template ast::ArgumentList Parser::internalParseArgumentList<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
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
extern template ExpressionPointer Parser::parsePostfixExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParsePostfixExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePostfixExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParsePostfixExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePostfixExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParsePostfixExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePostfixExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParsePostfixExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePostfixExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParsePostfixExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePostfixExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParsePostfixExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePostfixExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParsePostfixExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parsePostfixExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParsePostfixExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseUnaryExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseUnaryExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseUnaryExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseUnaryExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseUnaryExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseUnaryExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseUnaryExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseUnaryExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseUnaryExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseUnaryExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseUnaryExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseUnaryExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseUnaryExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseUnaryExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseUnaryExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseUnaryExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMultiplicativeExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseMultiplicativeExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMultiplicativeExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseMultiplicativeExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMultiplicativeExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseMultiplicativeExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMultiplicativeExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseMultiplicativeExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMultiplicativeExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseMultiplicativeExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMultiplicativeExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseMultiplicativeExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMultiplicativeExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseMultiplicativeExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseMultiplicativeExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseMultiplicativeExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAdditiveExpression<false, false, false>();
extern template ExpressionPointer Parser::internalParseAdditiveExpression<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAdditiveExpression<false, false, true>();
extern template ExpressionPointer Parser::internalParseAdditiveExpression<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAdditiveExpression<false, true, false>();
extern template ExpressionPointer Parser::internalParseAdditiveExpression<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAdditiveExpression<false, true, true>();
extern template ExpressionPointer Parser::internalParseAdditiveExpression<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAdditiveExpression<true, false, false>();
extern template ExpressionPointer Parser::internalParseAdditiveExpression<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAdditiveExpression<true, false, true>();
extern template ExpressionPointer Parser::internalParseAdditiveExpression<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAdditiveExpression<true, true, false>();
extern template ExpressionPointer Parser::internalParseAdditiveExpression<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ExpressionPointer Parser::parseAdditiveExpression<true, true, true>();
extern template ExpressionPointer Parser::internalParseAdditiveExpression<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
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
extern template StatementPointer Parser::parseContinueStatement<false, false, false>();
extern template StatementPointer Parser::internalParseContinueStatement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseContinueStatement<false, false, true>();
extern template StatementPointer Parser::internalParseContinueStatement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseContinueStatement<false, true, false>();
extern template StatementPointer Parser::internalParseContinueStatement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseContinueStatement<false, true, true>();
extern template StatementPointer Parser::internalParseContinueStatement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseContinueStatement<true, false, false>();
extern template StatementPointer Parser::internalParseContinueStatement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseContinueStatement<true, false, true>();
extern template StatementPointer Parser::internalParseContinueStatement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseContinueStatement<true, true, false>();
extern template StatementPointer Parser::internalParseContinueStatement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseContinueStatement<true, true, true>();
extern template StatementPointer Parser::internalParseContinueStatement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakStatement<false, false, false>();
extern template StatementPointer Parser::internalParseBreakStatement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakStatement<false, false, true>();
extern template StatementPointer Parser::internalParseBreakStatement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakStatement<false, true, false>();
extern template StatementPointer Parser::internalParseBreakStatement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakStatement<false, true, true>();
extern template StatementPointer Parser::internalParseBreakStatement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakStatement<true, false, false>();
extern template StatementPointer Parser::internalParseBreakStatement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakStatement<true, false, true>();
extern template StatementPointer Parser::internalParseBreakStatement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakStatement<true, true, false>();
extern template StatementPointer Parser::internalParseBreakStatement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakStatement<true, true, true>();
extern template StatementPointer Parser::internalParseBreakStatement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseReturnStatement<false, false, false>();
extern template StatementPointer Parser::internalParseReturnStatement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseReturnStatement<false, false, true>();
extern template StatementPointer Parser::internalParseReturnStatement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseReturnStatement<false, true, false>();
extern template StatementPointer Parser::internalParseReturnStatement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseReturnStatement<false, true, true>();
extern template StatementPointer Parser::internalParseReturnStatement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseReturnStatement<true, false, false>();
extern template StatementPointer Parser::internalParseReturnStatement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseReturnStatement<true, false, true>();
extern template StatementPointer Parser::internalParseReturnStatement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseReturnStatement<true, true, false>();
extern template StatementPointer Parser::internalParseReturnStatement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseReturnStatement<true, true, true>();
extern template StatementPointer Parser::internalParseReturnStatement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseWithStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseWithStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseWithStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseWithStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseWithStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseWithStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseWithStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseWithStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseWithStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseWithStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseWithStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseWithStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseWithStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseWithStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseWithStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseWithStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseWithStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseLabelledStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseLabelledStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseLabelledStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseLabelledStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseLabelledStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseLabelledStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseLabelledStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseLabelledStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseLabelledStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseLabelledStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseLabelledStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseLabelledStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseLabelledStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseLabelledStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseLabelledStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLabelledStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseLabelledStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseThrowStatement<false, false, false>();
extern template StatementPointer Parser::internalParseThrowStatement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseThrowStatement<false, false, true>();
extern template StatementPointer Parser::internalParseThrowStatement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseThrowStatement<false, true, false>();
extern template StatementPointer Parser::internalParseThrowStatement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseThrowStatement<false, true, true>();
extern template StatementPointer Parser::internalParseThrowStatement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseThrowStatement<true, false, false>();
extern template StatementPointer Parser::internalParseThrowStatement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseThrowStatement<true, false, true>();
extern template StatementPointer Parser::internalParseThrowStatement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseThrowStatement<true, true, false>();
extern template StatementPointer Parser::internalParseThrowStatement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseThrowStatement<true, true, true>();
extern template StatementPointer Parser::internalParseThrowStatement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseTryStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseTryStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseTryStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseTryStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseTryStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseTryStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseTryStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseTryStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseTryStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseTryStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseTryStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseTryStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseTryStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseTryStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseTryStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseTryStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseTryStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseDeclaration<false, false, false>();
extern template StatementPointer Parser::internalParseDeclaration<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseDeclaration<false, false, true>();
extern template StatementPointer Parser::internalParseDeclaration<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseDeclaration<false, true, false>();
extern template StatementPointer Parser::internalParseDeclaration<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseDeclaration<false, true, true>();
extern template StatementPointer Parser::internalParseDeclaration<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseDeclaration<true, false, false>();
extern template StatementPointer Parser::internalParseDeclaration<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseDeclaration<true, false, true>();
extern template StatementPointer Parser::internalParseDeclaration<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseDeclaration<true, true, false>();
extern template StatementPointer Parser::internalParseDeclaration<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseDeclaration<true, true, true>();
extern template StatementPointer Parser::internalParseDeclaration<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<false, false, false, false>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<false, false, false, true>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<false, false, true, false>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<false, false, true, true>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<false, true, false, false>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<false, true, false, true>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<false, true, true, false>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<false, true, true, true>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<true, false, false, false>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<true, false, false, true>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<true, false, true, false>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<true, false, true, true>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<true, true, false, false>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<true, true, false, true>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<true, true, true, false>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseHoistableDeclaration<true, true, true, true>();
extern template StatementPointer Parser::internalParseHoistableDeclaration<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<false, false, false, false>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<false, false, false, true>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<false, false, true, false>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<false, false, true, true>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<false, true, false, false>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<false, true, false, true>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<false, true, true, false>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<false, true, true, true>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<true, false, false, false>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<true, false, false, true>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<true, false, true, false>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<true, false, true, true>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<true, true, false, false>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<true, true, false, true>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<true, true, true, false>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseFunctionDeclaration<true, true, true, true>();
extern template StatementPointer Parser::internalParseFunctionDeclaration<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<false, false, false, false>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<false, false, false, true>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<false, false, true, false>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<false, false, true, true>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<false, true, false, false>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<false, true, false, true>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<false, true, true, false>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<false, true, true, true>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<true, false, false, false>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<true, false, false, true>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<true, false, true, false>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<true, false, true, true>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<true, true, false, false>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<true, true, false, true>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<true, true, true, false>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseGeneratorDeclaration<true, true, true, true>();
extern template StatementPointer Parser::internalParseGeneratorDeclaration<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<false, false, false, false>();
extern template StatementPointer Parser::internalParseClassDeclaration<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<false, false, false, true>();
extern template StatementPointer Parser::internalParseClassDeclaration<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<false, false, true, false>();
extern template StatementPointer Parser::internalParseClassDeclaration<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<false, false, true, true>();
extern template StatementPointer Parser::internalParseClassDeclaration<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<false, true, false, false>();
extern template StatementPointer Parser::internalParseClassDeclaration<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<false, true, false, true>();
extern template StatementPointer Parser::internalParseClassDeclaration<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<false, true, true, false>();
extern template StatementPointer Parser::internalParseClassDeclaration<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<false, true, true, true>();
extern template StatementPointer Parser::internalParseClassDeclaration<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<true, false, false, false>();
extern template StatementPointer Parser::internalParseClassDeclaration<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<true, false, false, true>();
extern template StatementPointer Parser::internalParseClassDeclaration<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<true, false, true, false>();
extern template StatementPointer Parser::internalParseClassDeclaration<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<true, false, true, true>();
extern template StatementPointer Parser::internalParseClassDeclaration<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<true, true, false, false>();
extern template StatementPointer Parser::internalParseClassDeclaration<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<true, true, false, true>();
extern template StatementPointer Parser::internalParseClassDeclaration<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<true, true, true, false>();
extern template StatementPointer Parser::internalParseClassDeclaration<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseClassDeclaration<true, true, true, true>();
extern template StatementPointer Parser::internalParseClassDeclaration<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseSwitchStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseSwitchStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseSwitchStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseSwitchStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseSwitchStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseSwitchStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseSwitchStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseSwitchStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseSwitchStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseSwitchStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseSwitchStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseSwitchStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseSwitchStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseSwitchStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseSwitchStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseSwitchStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseSwitchStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseBreakableStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseBreakableStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseBreakableStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseBreakableStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseBreakableStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseBreakableStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseBreakableStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseBreakableStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseBreakableStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseBreakableStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseBreakableStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseBreakableStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseBreakableStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseBreakableStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseBreakableStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBreakableStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseBreakableStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseBlockStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseBlockStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseBlockStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseBlockStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseBlockStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseBlockStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseBlockStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseBlockStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseBlockStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseBlockStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseBlockStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseBlockStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseBlockStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseBlockStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseBlockStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlockStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseBlockStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<false, false, false, false>();
extern template StatementPointer Parser::internalParseBlock<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<false, false, false, true>();
extern template StatementPointer Parser::internalParseBlock<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<false, false, true, false>();
extern template StatementPointer Parser::internalParseBlock<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<false, false, true, true>();
extern template StatementPointer Parser::internalParseBlock<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<false, true, false, false>();
extern template StatementPointer Parser::internalParseBlock<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<false, true, false, true>();
extern template StatementPointer Parser::internalParseBlock<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<false, true, true, false>();
extern template StatementPointer Parser::internalParseBlock<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<false, true, true, true>();
extern template StatementPointer Parser::internalParseBlock<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<true, false, false, false>();
extern template StatementPointer Parser::internalParseBlock<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<true, false, false, true>();
extern template StatementPointer Parser::internalParseBlock<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<true, false, true, false>();
extern template StatementPointer Parser::internalParseBlock<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<true, false, true, true>();
extern template StatementPointer Parser::internalParseBlock<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<true, true, false, false>();
extern template StatementPointer Parser::internalParseBlock<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<true, true, false, true>();
extern template StatementPointer Parser::internalParseBlock<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<true, true, true, false>();
extern template StatementPointer Parser::internalParseBlock<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseBlock<true, true, true, true>();
extern template StatementPointer Parser::internalParseBlock<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<false, false, false, false>();
extern template ast::StatementList Parser::internalParseStatementList<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<false, false, false, true>();
extern template ast::StatementList Parser::internalParseStatementList<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<false, false, true, false>();
extern template ast::StatementList Parser::internalParseStatementList<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<false, false, true, true>();
extern template ast::StatementList Parser::internalParseStatementList<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<false, true, false, false>();
extern template ast::StatementList Parser::internalParseStatementList<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<false, true, false, true>();
extern template ast::StatementList Parser::internalParseStatementList<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<false, true, true, false>();
extern template ast::StatementList Parser::internalParseStatementList<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<false, true, true, true>();
extern template ast::StatementList Parser::internalParseStatementList<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<true, false, false, false>();
extern template ast::StatementList Parser::internalParseStatementList<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<true, false, false, true>();
extern template ast::StatementList Parser::internalParseStatementList<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<true, false, true, false>();
extern template ast::StatementList Parser::internalParseStatementList<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<true, false, true, true>();
extern template ast::StatementList Parser::internalParseStatementList<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<true, true, false, false>();
extern template ast::StatementList Parser::internalParseStatementList<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<true, true, false, true>();
extern template ast::StatementList Parser::internalParseStatementList<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<true, true, true, false>();
extern template ast::StatementList Parser::internalParseStatementList<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::StatementList Parser::parseStatementList<true, true, true, true>();
extern template ast::StatementList Parser::internalParseStatementList<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<false, false, false, false>();
extern template StatementPointer Parser::internalParseStatementListItem<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<false, false, false, true>();
extern template StatementPointer Parser::internalParseStatementListItem<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<false, false, true, false>();
extern template StatementPointer Parser::internalParseStatementListItem<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<false, false, true, true>();
extern template StatementPointer Parser::internalParseStatementListItem<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<false, true, false, false>();
extern template StatementPointer Parser::internalParseStatementListItem<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<false, true, false, true>();
extern template StatementPointer Parser::internalParseStatementListItem<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<false, true, true, false>();
extern template StatementPointer Parser::internalParseStatementListItem<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<false, true, true, true>();
extern template StatementPointer Parser::internalParseStatementListItem<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<true, false, false, false>();
extern template StatementPointer Parser::internalParseStatementListItem<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<true, false, false, true>();
extern template StatementPointer Parser::internalParseStatementListItem<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<true, false, true, false>();
extern template StatementPointer Parser::internalParseStatementListItem<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<true, false, true, true>();
extern template StatementPointer Parser::internalParseStatementListItem<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<true, true, false, false>();
extern template StatementPointer Parser::internalParseStatementListItem<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<true, true, false, true>();
extern template StatementPointer Parser::internalParseStatementListItem<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<true, true, true, false>();
extern template StatementPointer Parser::internalParseStatementListItem<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseStatementListItem<true, true, true, true>();
extern template StatementPointer Parser::internalParseStatementListItem<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, false, false, false, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, false, false, false, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, false, false, true, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, false, false, true, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, false, true, false, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, false, true, false, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, false, true, true, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, false, true, true, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, true, false, false, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, true, false, false, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, true, false, true, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, true, false, true, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, true, true, false, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, true, true, false, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, true, true, true, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<false, true, true, true, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<false, true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, false, false, false, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, false, false, false, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, false, false, true, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, false, false, true, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, false, true, false, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, false, true, false, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, false, true, true, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, false, true, true, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, true, false, false, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, true, false, false, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, true, false, true, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, true, false, true, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, true, true, false, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, true, true, false, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, true, true, true, false>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseLexicalDeclaration<true, true, true, true, true>();
extern template StatementPointer Parser::internalParseLexicalDeclaration<true, true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<false, false, false, false>();
extern template ast::BindingList Parser::internalParseBindingList<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<false, false, false, true>();
extern template ast::BindingList Parser::internalParseBindingList<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<false, false, true, false>();
extern template ast::BindingList Parser::internalParseBindingList<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<false, false, true, true>();
extern template ast::BindingList Parser::internalParseBindingList<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<false, true, false, false>();
extern template ast::BindingList Parser::internalParseBindingList<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<false, true, false, true>();
extern template ast::BindingList Parser::internalParseBindingList<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<false, true, true, false>();
extern template ast::BindingList Parser::internalParseBindingList<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<false, true, true, true>();
extern template ast::BindingList Parser::internalParseBindingList<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<true, false, false, false>();
extern template ast::BindingList Parser::internalParseBindingList<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<true, false, false, true>();
extern template ast::BindingList Parser::internalParseBindingList<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<true, false, true, false>();
extern template ast::BindingList Parser::internalParseBindingList<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<true, false, true, true>();
extern template ast::BindingList Parser::internalParseBindingList<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<true, true, false, false>();
extern template ast::BindingList Parser::internalParseBindingList<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<true, true, false, true>();
extern template ast::BindingList Parser::internalParseBindingList<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<true, true, true, false>();
extern template ast::BindingList Parser::internalParseBindingList<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingList<true, true, true, true>();
extern template ast::BindingList Parser::internalParseBindingList<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<false, false, false, false>();
extern template BindingPointer Parser::internalParseLexicalBinding<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<false, false, false, true>();
extern template BindingPointer Parser::internalParseLexicalBinding<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<false, false, true, false>();
extern template BindingPointer Parser::internalParseLexicalBinding<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<false, false, true, true>();
extern template BindingPointer Parser::internalParseLexicalBinding<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<false, true, false, false>();
extern template BindingPointer Parser::internalParseLexicalBinding<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<false, true, false, true>();
extern template BindingPointer Parser::internalParseLexicalBinding<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<false, true, true, false>();
extern template BindingPointer Parser::internalParseLexicalBinding<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<false, true, true, true>();
extern template BindingPointer Parser::internalParseLexicalBinding<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<true, false, false, false>();
extern template BindingPointer Parser::internalParseLexicalBinding<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<true, false, false, true>();
extern template BindingPointer Parser::internalParseLexicalBinding<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<true, false, true, false>();
extern template BindingPointer Parser::internalParseLexicalBinding<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<true, false, true, true>();
extern template BindingPointer Parser::internalParseLexicalBinding<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<true, true, false, false>();
extern template BindingPointer Parser::internalParseLexicalBinding<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<true, true, false, true>();
extern template BindingPointer Parser::internalParseLexicalBinding<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<true, true, true, false>();
extern template BindingPointer Parser::internalParseLexicalBinding<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseLexicalBinding<true, true, true, true>();
extern template BindingPointer Parser::internalParseLexicalBinding<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseVariableStatement<false, false, false>();
extern template StatementPointer Parser::internalParseVariableStatement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseVariableStatement<false, false, true>();
extern template StatementPointer Parser::internalParseVariableStatement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseVariableStatement<false, true, false>();
extern template StatementPointer Parser::internalParseVariableStatement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseVariableStatement<false, true, true>();
extern template StatementPointer Parser::internalParseVariableStatement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseVariableStatement<true, false, false>();
extern template StatementPointer Parser::internalParseVariableStatement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseVariableStatement<true, false, true>();
extern template StatementPointer Parser::internalParseVariableStatement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseVariableStatement<true, true, false>();
extern template StatementPointer Parser::internalParseVariableStatement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseVariableStatement<true, true, true>();
extern template StatementPointer Parser::internalParseVariableStatement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<false, false, false, false>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<false, false, false, true>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<false, false, true, false>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<false, false, true, true>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<false, true, false, false>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<false, true, false, true>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<false, true, true, false>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<false, true, true, true>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<true, false, false, false>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<true, false, false, true>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<true, false, true, false>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<true, false, true, true>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<true, true, false, false>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<true, true, false, true>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<true, true, true, false>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseVariableDeclarationList<true, true, true, true>();
extern template ast::BindingList Parser::internalParseVariableDeclarationList<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<false, false, false, false>();
extern template BindingPointer Parser::internalParseVariableDeclaration<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<false, false, false, true>();
extern template BindingPointer Parser::internalParseVariableDeclaration<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<false, false, true, false>();
extern template BindingPointer Parser::internalParseVariableDeclaration<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<false, false, true, true>();
extern template BindingPointer Parser::internalParseVariableDeclaration<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<false, true, false, false>();
extern template BindingPointer Parser::internalParseVariableDeclaration<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<false, true, false, true>();
extern template BindingPointer Parser::internalParseVariableDeclaration<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<false, true, true, false>();
extern template BindingPointer Parser::internalParseVariableDeclaration<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<false, true, true, true>();
extern template BindingPointer Parser::internalParseVariableDeclaration<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<true, false, false, false>();
extern template BindingPointer Parser::internalParseVariableDeclaration<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<true, false, false, true>();
extern template BindingPointer Parser::internalParseVariableDeclaration<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<true, false, true, false>();
extern template BindingPointer Parser::internalParseVariableDeclaration<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<true, false, true, true>();
extern template BindingPointer Parser::internalParseVariableDeclaration<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<true, true, false, false>();
extern template BindingPointer Parser::internalParseVariableDeclaration<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<true, true, false, true>();
extern template BindingPointer Parser::internalParseVariableDeclaration<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<true, true, true, false>();
extern template BindingPointer Parser::internalParseVariableDeclaration<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseVariableDeclaration<true, true, true, true>();
extern template BindingPointer Parser::internalParseVariableDeclaration<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingPattern<false, false, false>();
extern template BindingPointer Parser::internalParseBindingPattern<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingPattern<false, false, true>();
extern template BindingPointer Parser::internalParseBindingPattern<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingPattern<false, true, false>();
extern template BindingPointer Parser::internalParseBindingPattern<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingPattern<false, true, true>();
extern template BindingPointer Parser::internalParseBindingPattern<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingPattern<true, false, false>();
extern template BindingPointer Parser::internalParseBindingPattern<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingPattern<true, false, true>();
extern template BindingPointer Parser::internalParseBindingPattern<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingPattern<true, true, false>();
extern template BindingPointer Parser::internalParseBindingPattern<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingPattern<true, true, true>();
extern template BindingPointer Parser::internalParseBindingPattern<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseObjectBindingPattern<false, false, false>();
extern template BindingPointer Parser::internalParseObjectBindingPattern<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseObjectBindingPattern<false, false, true>();
extern template BindingPointer Parser::internalParseObjectBindingPattern<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseObjectBindingPattern<false, true, false>();
extern template BindingPointer Parser::internalParseObjectBindingPattern<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseObjectBindingPattern<false, true, true>();
extern template BindingPointer Parser::internalParseObjectBindingPattern<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseObjectBindingPattern<true, false, false>();
extern template BindingPointer Parser::internalParseObjectBindingPattern<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseObjectBindingPattern<true, false, true>();
extern template BindingPointer Parser::internalParseObjectBindingPattern<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseObjectBindingPattern<true, true, false>();
extern template BindingPointer Parser::internalParseObjectBindingPattern<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseObjectBindingPattern<true, true, true>();
extern template BindingPointer Parser::internalParseObjectBindingPattern<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseArrayBindingPattern<false, false, false>();
extern template BindingPointer Parser::internalParseArrayBindingPattern<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseArrayBindingPattern<false, false, true>();
extern template BindingPointer Parser::internalParseArrayBindingPattern<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseArrayBindingPattern<false, true, false>();
extern template BindingPointer Parser::internalParseArrayBindingPattern<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseArrayBindingPattern<false, true, true>();
extern template BindingPointer Parser::internalParseArrayBindingPattern<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseArrayBindingPattern<true, false, false>();
extern template BindingPointer Parser::internalParseArrayBindingPattern<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseArrayBindingPattern<true, false, true>();
extern template BindingPointer Parser::internalParseArrayBindingPattern<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseArrayBindingPattern<true, true, false>();
extern template BindingPointer Parser::internalParseArrayBindingPattern<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseArrayBindingPattern<true, true, true>();
extern template BindingPointer Parser::internalParseArrayBindingPattern<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseArrayBindingElementList<false, false, false>();
extern template ast::BindingList Parser::internalParseArrayBindingElementList<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseArrayBindingElementList<false, false, true>();
extern template ast::BindingList Parser::internalParseArrayBindingElementList<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseArrayBindingElementList<false, true, false>();
extern template ast::BindingList Parser::internalParseArrayBindingElementList<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseArrayBindingElementList<false, true, true>();
extern template ast::BindingList Parser::internalParseArrayBindingElementList<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseArrayBindingElementList<true, false, false>();
extern template ast::BindingList Parser::internalParseArrayBindingElementList<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseArrayBindingElementList<true, false, true>();
extern template ast::BindingList Parser::internalParseArrayBindingElementList<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseArrayBindingElementList<true, true, false>();
extern template ast::BindingList Parser::internalParseArrayBindingElementList<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseArrayBindingElementList<true, true, true>();
extern template ast::BindingList Parser::internalParseArrayBindingElementList<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingPropertyList<false, false, false>();
extern template ast::BindingList Parser::internalParseBindingPropertyList<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingPropertyList<false, false, true>();
extern template ast::BindingList Parser::internalParseBindingPropertyList<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingPropertyList<false, true, false>();
extern template ast::BindingList Parser::internalParseBindingPropertyList<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingPropertyList<false, true, true>();
extern template ast::BindingList Parser::internalParseBindingPropertyList<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingPropertyList<true, false, false>();
extern template ast::BindingList Parser::internalParseBindingPropertyList<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingPropertyList<true, false, true>();
extern template ast::BindingList Parser::internalParseBindingPropertyList<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingPropertyList<true, true, false>();
extern template ast::BindingList Parser::internalParseBindingPropertyList<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template ast::BindingList Parser::parseBindingPropertyList<true, true, true>();
extern template ast::BindingList Parser::internalParseBindingPropertyList<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingProperty<false, false, false>();
extern template BindingPointer Parser::internalParseBindingProperty<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingProperty<false, false, true>();
extern template BindingPointer Parser::internalParseBindingProperty<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingProperty<false, true, false>();
extern template BindingPointer Parser::internalParseBindingProperty<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingProperty<false, true, true>();
extern template BindingPointer Parser::internalParseBindingProperty<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingProperty<true, false, false>();
extern template BindingPointer Parser::internalParseBindingProperty<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingProperty<true, false, true>();
extern template BindingPointer Parser::internalParseBindingProperty<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingProperty<true, true, false>();
extern template BindingPointer Parser::internalParseBindingProperty<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingProperty<true, true, true>();
extern template BindingPointer Parser::internalParseBindingProperty<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingElement<false, false, false>();
extern template BindingPointer Parser::internalParseBindingElement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingElement<false, false, true>();
extern template BindingPointer Parser::internalParseBindingElement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingElement<false, true, false>();
extern template BindingPointer Parser::internalParseBindingElement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingElement<false, true, true>();
extern template BindingPointer Parser::internalParseBindingElement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingElement<true, false, false>();
extern template BindingPointer Parser::internalParseBindingElement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingElement<true, false, true>();
extern template BindingPointer Parser::internalParseBindingElement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingElement<true, true, false>();
extern template BindingPointer Parser::internalParseBindingElement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingElement<true, true, true>();
extern template BindingPointer Parser::internalParseBindingElement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingRestElement<false, false, false>();
extern template BindingPointer Parser::internalParseBindingRestElement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingRestElement<false, false, true>();
extern template BindingPointer Parser::internalParseBindingRestElement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingRestElement<false, true, false>();
extern template BindingPointer Parser::internalParseBindingRestElement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingRestElement<false, true, true>();
extern template BindingPointer Parser::internalParseBindingRestElement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingRestElement<true, false, false>();
extern template BindingPointer Parser::internalParseBindingRestElement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingRestElement<true, false, true>();
extern template BindingPointer Parser::internalParseBindingRestElement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingRestElement<true, true, false>();
extern template BindingPointer Parser::internalParseBindingRestElement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseBindingRestElement<true, true, true>();
extern template BindingPointer Parser::internalParseBindingRestElement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseSingleNameBinding<false, false, false>();
extern template BindingPointer Parser::internalParseSingleNameBinding<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseSingleNameBinding<false, false, true>();
extern template BindingPointer Parser::internalParseSingleNameBinding<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseSingleNameBinding<false, true, false>();
extern template BindingPointer Parser::internalParseSingleNameBinding<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseSingleNameBinding<false, true, true>();
extern template BindingPointer Parser::internalParseSingleNameBinding<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseSingleNameBinding<true, false, false>();
extern template BindingPointer Parser::internalParseSingleNameBinding<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseSingleNameBinding<true, false, true>();
extern template BindingPointer Parser::internalParseSingleNameBinding<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseSingleNameBinding<true, true, false>();
extern template BindingPointer Parser::internalParseSingleNameBinding<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template BindingPointer Parser::parseSingleNameBinding<true, true, true>();
extern template BindingPointer Parser::internalParseSingleNameBinding<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseExpressionStatement<false, false, false>();
extern template StatementPointer Parser::internalParseExpressionStatement<false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseExpressionStatement<false, false, true>();
extern template StatementPointer Parser::internalParseExpressionStatement<false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseExpressionStatement<false, true, false>();
extern template StatementPointer Parser::internalParseExpressionStatement<false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseExpressionStatement<false, true, true>();
extern template StatementPointer Parser::internalParseExpressionStatement<false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseExpressionStatement<true, false, false>();
extern template StatementPointer Parser::internalParseExpressionStatement<true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseExpressionStatement<true, false, true>();
extern template StatementPointer Parser::internalParseExpressionStatement<true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseExpressionStatement<true, true, false>();
extern template StatementPointer Parser::internalParseExpressionStatement<true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseExpressionStatement<true, true, true>();
extern template StatementPointer Parser::internalParseExpressionStatement<true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseIfStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseIfStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseIfStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseIfStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseIfStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseIfStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseIfStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseIfStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseIfStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseIfStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseIfStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseIfStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseIfStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseIfStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseIfStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIfStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseIfStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<false, false, false, false>();
extern template StatementPointer Parser::internalParseIterationStatement<false, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<false, false, false, true>();
extern template StatementPointer Parser::internalParseIterationStatement<false, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<false, false, true, false>();
extern template StatementPointer Parser::internalParseIterationStatement<false, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<false, false, true, true>();
extern template StatementPointer Parser::internalParseIterationStatement<false, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<false, true, false, false>();
extern template StatementPointer Parser::internalParseIterationStatement<false, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<false, true, false, true>();
extern template StatementPointer Parser::internalParseIterationStatement<false, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<false, true, true, false>();
extern template StatementPointer Parser::internalParseIterationStatement<false, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<false, true, true, true>();
extern template StatementPointer Parser::internalParseIterationStatement<false, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<true, false, false, false>();
extern template StatementPointer Parser::internalParseIterationStatement<true, false, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<true, false, false, true>();
extern template StatementPointer Parser::internalParseIterationStatement<true, false, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<true, false, true, false>();
extern template StatementPointer Parser::internalParseIterationStatement<true, false, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<true, false, true, true>();
extern template StatementPointer Parser::internalParseIterationStatement<true, false, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<true, true, false, false>();
extern template StatementPointer Parser::internalParseIterationStatement<true, true, false, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<true, true, false, true>();
extern template StatementPointer Parser::internalParseIterationStatement<true, true, false, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<true, true, true, false>();
extern template StatementPointer Parser::internalParseIterationStatement<true, true, true, false>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
extern template StatementPointer Parser::parseIterationStatement<true, true, true, true>();
extern template StatementPointer Parser::internalParseIterationStatement<true, true, true, true>(std::size_t startLocation, RuleResult &ruleResultOut, bool isRequiredForSuccess);
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_ */
