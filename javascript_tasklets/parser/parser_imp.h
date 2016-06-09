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

#ifndef JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_
#define JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_

#include "../value.h"
#include "source.h"
#include "parser.h"

namespace javascript_tasklets
{
namespace parser
{
struct Parser final : public gc::Object::ExtraData
{
    Parser(const Parser &) = delete;
    Parser &operator=(const Parser &) = delete;

public:
    struct RuleStatus final
    {
        static constexpr std::size_t npos = static_cast<std::size_t>(-1);
        std::size_t startPositionOrErrorPosition;
        std::size_t endPositionOrErrorPriorityPosition;
        const char16_t *errorMessage;
        constexpr bool success() const noexcept
        {
            return errorMessage == nullptr && startPositionOrErrorPosition != npos;
        }
        constexpr bool empty() const noexcept
        {
            return errorMessage == nullptr && startPositionOrErrorPosition == npos;
        }
        constexpr bool fail() const noexcept
        {
            return errorMessage != nullptr;
        }

    private:
        constexpr RuleStatus(std::size_t startPositionOrErrorPosition,
                             std::size_t endPositionOrErrorPriorityPosition,
                             const char16_t *errorMessage) noexcept
            : startPositionOrErrorPosition(startPositionOrErrorPosition),
              endPositionOrErrorPriorityPosition(endPositionOrErrorPriorityPosition),
              errorMessage(errorMessage)
        {
        }

    public:
        constexpr RuleStatus() noexcept : startPositionOrErrorPosition(npos),
                                          endPositionOrErrorPriorityPosition(npos),
                                          errorMessage(nullptr)
        {
        }
        static constexpr RuleStatus makeSuccess(std::size_t startPosition,
                                                std::size_t endPosition) noexcept
        {
            return constexpr_assert(startPosition <= endPosition && startPosition != npos
                                    && endPosition != npos),
                   RuleStatus(startPosition, endPosition, nullptr);
        }
        static constexpr RuleStatus makeFailure(std::size_t errorPosition,
                                                std::size_t errorPriorityPosition,
                                                const char16_t *errorMessage) noexcept
        {
            return constexpr_assert(errorMessage),
                   RuleStatus(errorPosition, errorPriorityPosition, errorMessage);
        }
        constexpr RuleStatus operator/(RuleStatus rt) noexcept
        {
            return constexpr_assert(fail() || rt.fail()), constexpr_assert(!empty() && !rt.empty()),
                   !fail() ? *this : !rt.fail() ? rt :
                                                  endPositionOrErrorPriorityPosition
                                                          < rt.endPositionOrErrorPriorityPosition ?
                                                  rt :
                                                  *this;
        }
        RuleStatus &operator/=(RuleStatus rt) noexcept
        {
            return operator=(operator/(rt));
        }
    };
    template <typename T>
    struct ArrayWithBoolIndex final
    {
        T falseValue;
        T trueValue;
        T &operator[](bool index)
        {
            return index ? trueValue : falseValue;
        }
        const T &operator[](bool index) const
        {
            return index ? trueValue : falseValue;
        }
    };
    template <typename T>
    using YieldOption = ArrayWithBoolIndex<T>;
    template <typename T>
    using ReturnOption = ArrayWithBoolIndex<T>;
    template <typename T>
    using InOption = ArrayWithBoolIndex<T>;
    struct RuleStatuses final
    {
        gc::StringReference identifierNameValue;
        double numericLiteralValue = 0;
        bool booleanLiteralValue = false;
        gc::StringReference stringLiteralValue;
        bool escapeInString = false;
        gc::StringReference regularExpressionLiteralValue;
        gc::StringReference regularExpressionLiteralFlagsValue;
        gc::StringReference templateRawValue;
        gc::StringReference templateValue;
        bool lineSplit = false;
        RuleStatus tokenizationIdentifierNameStatus;
        RuleStatus tokenizationIdentifierStatus;
        RuleStatus tokenizationEscapelessIdentifierNameStatus;
        RuleStatus tokenizationStringLiteralStatus;
        RuleStatus tokenizationSeperatorStatus;
        RuleStatus tokenizationKeywordStatus;
        RuleStatus tokenizationFutureReservedWordStatus;
        RuleStatus tokenizationBooleanLiteralStatus;
        RuleStatus tokenizationReservedWordStatus;
        RuleStatus tokenizationNumericLiteralStatus;
        RuleStatus tokenizationRegularExpressionLiteralStatus;
        RuleStatus tokenizationTemplateCharactersStatus;
        RuleStatus tokenizationNoSubstitutionTemplateStatus;
        RuleStatus tokenizationTemplateHeadStatus;
        RuleStatus tokenizationTemplateTailStatus;
        RuleStatus tokenizationTemplateMiddleStatus;
        RuleStatus directivePrologueStatus;
        bool directivePrologueHasUseStrict = false;
        InOption<YieldOption<RuleStatus>> relationalExpressionStatus;
        InOption<YieldOption<RuleStatus>> equalityExpressionStatus;
        InOption<YieldOption<RuleStatus>> bitwiseAndExpressionStatus;
        InOption<YieldOption<RuleStatus>> bitwiseXorExpressionStatus;
        InOption<YieldOption<RuleStatus>> bitwiseOrExpressionStatus;
        InOption<YieldOption<RuleStatus>> logicalAndExpressionStatus;
        InOption<YieldOption<RuleStatus>> logicalOrExpressionStatus;
        InOption<YieldOption<RuleStatus>> conditionalExpressionStatus;
        InOption<YieldOption<RuleStatus>> assignmentExpressionStatus;
        InOption<YieldOption<RuleStatus>> expressionStatus;
        YieldOption<ReturnOption<RuleStatus>> statementStatus;
        YieldOption<ReturnOption<RuleStatus>> blockStatus;
        YieldOption<ReturnOption<RuleStatus>> statementListStatus;
        YieldOption<ReturnOption<RuleStatus>> statementListItemStatus;
        YieldOption<RuleStatus> variableStatementStatus;
        InOption<YieldOption<RuleStatus>> variableDeclarationListStatus;
        YieldOption<RuleStatus> expressionStatementStatus;
        RuleStatus scriptStatus;
        void getGCReferences(gc::GCReferencesCallback &callback) const
        {
            callback(identifierNameValue);
            callback(stringLiteralValue);
            callback(regularExpressionLiteralValue);
            callback(regularExpressionLiteralFlagsValue);
            callback(templateRawValue);
            callback(templateValue);
        }
    };

public:
    static constexpr std::uint32_t eofCodePoint = 0xFFFFFFFFUL;

public:
    ASTTranslator &astTranslator;
    std::vector<std::unique_ptr<RuleStatuses>> ruleStatusesArray;
    gc::SourceReference source;
    std::size_t currentPosition;

public:
    explicit Parser(ASTTranslator &astTranslator, SourceHandle source);
    virtual std::unique_ptr<ExtraData> clone() const override;
    virtual void getGCReferences(gc::GCReferencesCallback &callback) const override;
    RuleStatuses &getRuleStatuses(std::size_t position);
    std::uint32_t getCodePoint(std::size_t position);
    std::uint32_t getCodePoint(std::size_t position, std::size_t &nextPosition);
    RuleStatus parseTokenizationLineTerminatorSequence(GC &gc);
    RuleStatus parseTokenizationSeperator(GC &gc);
    RuleStatus parseTokenizationUnicodeEscapeSequence(GC &gc, std::uint32_t &value);
    RuleStatus parseTokenizationHexEscapeSequence(GC &gc, std::uint32_t &value);
    RuleStatus parseTokenizationEscapeSequence(GC &gc, std::uint32_t &value);
    RuleStatus parseTokenizationIdentifierName(GC &gc);
    RuleStatus parseTokenizationEscapelessIdentifierName(GC &gc);
    RuleStatus parseTokenizationKeyword(GC &gc, const char16_t *text, const char16_t *errorMessage);
    RuleStatus parseTokenizationAwait(GC &gc);
    RuleStatus parseTokenizationBreak(GC &gc);
    RuleStatus parseTokenizationCase(GC &gc);
    RuleStatus parseTokenizationCatch(GC &gc);
    RuleStatus parseTokenizationClass(GC &gc);
    RuleStatus parseTokenizationConst(GC &gc);
    RuleStatus parseTokenizationContinue(GC &gc);
    RuleStatus parseTokenizationDebugger(GC &gc);
    RuleStatus parseTokenizationDefault(GC &gc);
    RuleStatus parseTokenizationDelete(GC &gc);
    RuleStatus parseTokenizationDo(GC &gc);
    RuleStatus parseTokenizationElse(GC &gc);
    RuleStatus parseTokenizationEnum(GC &gc);
    RuleStatus parseTokenizationExport(GC &gc);
    RuleStatus parseTokenizationExtends(GC &gc);
    RuleStatus parseTokenizationFalse(GC &gc);
    RuleStatus parseTokenizationFinally(GC &gc);
    RuleStatus parseTokenizationFor(GC &gc);
    RuleStatus parseTokenizationFunction(GC &gc);
    RuleStatus parseTokenizationIf(GC &gc);
    RuleStatus parseTokenizationImport(GC &gc);
    RuleStatus parseTokenizationIn(GC &gc);
    RuleStatus parseTokenizationInstanceOf(GC &gc);
    RuleStatus parseTokenizationLet(GC &gc);
    RuleStatus parseTokenizationNew(GC &gc);
    RuleStatus parseTokenizationNull(GC &gc);
    RuleStatus parseTokenizationReturn(GC &gc);
    RuleStatus parseTokenizationSuper(GC &gc);
    RuleStatus parseTokenizationSwitch(GC &gc);
    RuleStatus parseTokenizationThis(GC &gc);
    RuleStatus parseTokenizationThrow(GC &gc);
    RuleStatus parseTokenizationTrue(GC &gc);
    RuleStatus parseTokenizationTry(GC &gc);
    RuleStatus parseTokenizationTypeOf(GC &gc);
    RuleStatus parseTokenizationVar(GC &gc);
    RuleStatus parseTokenizationVoid(GC &gc);
    RuleStatus parseTokenizationWhile(GC &gc);
    RuleStatus parseTokenizationWith(GC &gc);
    RuleStatus parseTokenizationYield(GC &gc);
    RuleStatus parseTokenizationKeyword(GC &gc);
    RuleStatus parseTokenizationFutureReservedWord(GC &gc);
    RuleStatus parseTokenizationBooleanLiteral(GC &gc);
    RuleStatus parseTokenizationReservedWord(GC &gc);
    RuleStatus parseTokenizationIdentifier(GC &gc);
    RuleStatus parseTokenizationNumericLiteral(GC &gc);
    RuleStatus parseTokenizationStringLiteral(GC &gc);
    RuleStatus parseTokenizationRegularExpressionLiteral(GC &gc);
    RuleStatus parseTokenizationTemplateCharacters(GC &gc);
    RuleStatus parseTokenizationNoSubstitutionTemplate(GC &gc);
    RuleStatus parseTokenizationTemplateHead(GC &gc);
    RuleStatus parseTokenizationTemplateTail(GC &gc);
    RuleStatus parseTokenizationTemplateMiddle(GC &gc);
    enum class Punctuator
    {
        LBrace,
        RBrace,
        LParen,
        RParen,
        LBracket,
        RBracket,
        Period,
        Ellipsis,
        Semicolon,
        Comma,
        LAngle,
        RAngle,
        LAngleEqual,
        RAngleEqual,
        EqualEqual,
        EMarkEqual,
        EqualEqualEqual,
        EMarkEqualEqual,
        Plus,
        Minus,
        Star,
        Percent,
        PlusPlus,
        MinusMinus,
        LAngleLAngle,
        RAngleRAngle,
        RAngleRAngleRAngle,
        Amp,
        Pipe,
        Caret,
        EMark,
        Tilde,
        AmpAmp,
        PipePipe,
        QMark,
        Colon,
        Equal,
        PlusEqual,
        MinusEqual,
        StarEqual,
        PercentEqual,
        LAngleLAngleEqual,
        RAngleRAngleEqual,
        RAngleRAngleRAngleEqual,
        AmpEqual,
        PipeEqual,
        CaretEqual,
        EqualRAngle,
        FSlash,
        FSlashEqual,
    };
    RuleStatus parseTokenizationPunctuator(GC &gc, Punctuator &punctuator);
    RuleStatus parseTokenizationPunctuator(GC &gc,
                                           Punctuator correctPunctuator,
                                           const char16_t *failMessage);
    RuleStatus parseTokenizationLBrace(GC &gc);
    RuleStatus parseTokenizationRBrace(GC &gc);
    RuleStatus parseTokenizationLParen(GC &gc);
    RuleStatus parseTokenizationRParen(GC &gc);
    RuleStatus parseTokenizationLBracket(GC &gc);
    RuleStatus parseTokenizationRBracket(GC &gc);
    RuleStatus parseTokenizationPeriod(GC &gc);
    RuleStatus parseTokenizationEllipsis(GC &gc);
    RuleStatus parseTokenizationSemicolon(GC &gc);
    RuleStatus parseTokenizationComma(GC &gc);
    RuleStatus parseTokenizationLAngle(GC &gc);
    RuleStatus parseTokenizationRAngle(GC &gc);
    RuleStatus parseTokenizationLAngleEqual(GC &gc);
    RuleStatus parseTokenizationRAngleEqual(GC &gc);
    RuleStatus parseTokenizationEqualEqual(GC &gc);
    RuleStatus parseTokenizationEMarkEqual(GC &gc);
    RuleStatus parseTokenizationEqualEqualEqual(GC &gc);
    RuleStatus parseTokenizationEMarkEqualEqual(GC &gc);
    RuleStatus parseTokenizationPlus(GC &gc);
    RuleStatus parseTokenizationMinus(GC &gc);
    RuleStatus parseTokenizationStar(GC &gc);
    RuleStatus parseTokenizationPercent(GC &gc);
    RuleStatus parseTokenizationPlusPlus(GC &gc);
    RuleStatus parseTokenizationMinusMinus(GC &gc);
    RuleStatus parseTokenizationLAngleLAngle(GC &gc);
    RuleStatus parseTokenizationRAngleRAngle(GC &gc);
    RuleStatus parseTokenizationRAngleRAngleRAngle(GC &gc);
    RuleStatus parseTokenizationAmp(GC &gc);
    RuleStatus parseTokenizationPipe(GC &gc);
    RuleStatus parseTokenizationCaret(GC &gc);
    RuleStatus parseTokenizationEMark(GC &gc);
    RuleStatus parseTokenizationTilde(GC &gc);
    RuleStatus parseTokenizationAmpAmp(GC &gc);
    RuleStatus parseTokenizationPipePipe(GC &gc);
    RuleStatus parseTokenizationQMark(GC &gc);
    RuleStatus parseTokenizationColon(GC &gc);
    RuleStatus parseTokenizationEqual(GC &gc);
    RuleStatus parseTokenizationPlusEqual(GC &gc);
    RuleStatus parseTokenizationMinusEqual(GC &gc);
    RuleStatus parseTokenizationStarEqual(GC &gc);
    RuleStatus parseTokenizationPercentEqual(GC &gc);
    RuleStatus parseTokenizationLAngleLAngleEqual(GC &gc);
    RuleStatus parseTokenizationRAngleRAngleEqual(GC &gc);
    RuleStatus parseTokenizationRAngleRAngleRAngleEqual(GC &gc);
    RuleStatus parseTokenizationAmpEqual(GC &gc);
    RuleStatus parseTokenizationPipeEqual(GC &gc);
    RuleStatus parseTokenizationCaretEqual(GC &gc);
    RuleStatus parseTokenizationEqualRAngle(GC &gc);
    RuleStatus parseTokenizationFSlash(GC &gc);
    RuleStatus parseTokenizationFSlashEqual(GC &gc);
    RuleStatus parseTokenizationEndOfFile(GC &gc);
    bool testParseToken(bool parseTemplateContinuation, bool parseRegularExpression, GC &gc);
    template <RuleStatus (Parser::*parseTokenization)(GC &gc)>
    RuleStatus parseSeperatorAndToken(GC &gc,
                                      void (*finalCopy)(RuleStatuses &dest,
                                                        RuleStatuses &src) = nullptr);
    RuleStatus parseTokenIdentifierName(GC &gc);
    RuleStatus parseTokenIdentifier(GC &gc);
    RuleStatus parseTokenLBrace(GC &gc);
    RuleStatus parseTokenRBrace(GC &gc);
    RuleStatus parseTokenLParen(GC &gc);
    RuleStatus parseTokenRParen(GC &gc);
    RuleStatus parseTokenLBracket(GC &gc);
    RuleStatus parseTokenRBracket(GC &gc);
    RuleStatus parseTokenPeriod(GC &gc);
    RuleStatus parseTokenEllipsis(GC &gc);
    RuleStatus parseTokenSemicolon(GC &gc);
    RuleStatus parseTokenComma(GC &gc);
    RuleStatus parseTokenLAngle(GC &gc);
    RuleStatus parseTokenRAngle(GC &gc);
    RuleStatus parseTokenLAngleEqual(GC &gc);
    RuleStatus parseTokenRAngleEqual(GC &gc);
    RuleStatus parseTokenEqualEqual(GC &gc);
    RuleStatus parseTokenEMarkEqual(GC &gc);
    RuleStatus parseTokenEqualEqualEqual(GC &gc);
    RuleStatus parseTokenEMarkEqualEqual(GC &gc);
    RuleStatus parseTokenPlus(GC &gc);
    RuleStatus parseTokenMinus(GC &gc);
    RuleStatus parseTokenStar(GC &gc);
    RuleStatus parseTokenPercent(GC &gc);
    RuleStatus parseTokenPlusPlus(GC &gc);
    RuleStatus parseTokenMinusMinus(GC &gc);
    RuleStatus parseTokenLAngleLAngle(GC &gc);
    RuleStatus parseTokenRAngleRAngle(GC &gc);
    RuleStatus parseTokenRAngleRAngleRAngle(GC &gc);
    RuleStatus parseTokenAmp(GC &gc);
    RuleStatus parseTokenPipe(GC &gc);
    RuleStatus parseTokenCaret(GC &gc);
    RuleStatus parseTokenEMark(GC &gc);
    RuleStatus parseTokenTilde(GC &gc);
    RuleStatus parseTokenAmpAmp(GC &gc);
    RuleStatus parseTokenPipePipe(GC &gc);
    RuleStatus parseTokenQMark(GC &gc);
    RuleStatus parseTokenColon(GC &gc);
    RuleStatus parseTokenEqual(GC &gc);
    RuleStatus parseTokenPlusEqual(GC &gc);
    RuleStatus parseTokenMinusEqual(GC &gc);
    RuleStatus parseTokenStarEqual(GC &gc);
    RuleStatus parseTokenPercentEqual(GC &gc);
    RuleStatus parseTokenLAngleLAngleEqual(GC &gc);
    RuleStatus parseTokenRAngleRAngleEqual(GC &gc);
    RuleStatus parseTokenRAngleRAngleRAngleEqual(GC &gc);
    RuleStatus parseTokenAmpEqual(GC &gc);
    RuleStatus parseTokenPipeEqual(GC &gc);
    RuleStatus parseTokenCaretEqual(GC &gc);
    RuleStatus parseTokenEqualRAngle(GC &gc);
    RuleStatus parseTokenFSlash(GC &gc);
    RuleStatus parseTokenFSlashEqual(GC &gc);
    RuleStatus parseTokenAwait(GC &gc);
    RuleStatus parseTokenBreak(GC &gc);
    RuleStatus parseTokenCase(GC &gc);
    RuleStatus parseTokenCatch(GC &gc);
    RuleStatus parseTokenClass(GC &gc);
    RuleStatus parseTokenConst(GC &gc);
    RuleStatus parseTokenContinue(GC &gc);
    RuleStatus parseTokenDebugger(GC &gc);
    RuleStatus parseTokenDefault(GC &gc);
    RuleStatus parseTokenDelete(GC &gc);
    RuleStatus parseTokenDo(GC &gc);
    RuleStatus parseTokenElse(GC &gc);
    RuleStatus parseTokenEnum(GC &gc);
    RuleStatus parseTokenExport(GC &gc);
    RuleStatus parseTokenExtends(GC &gc);
    RuleStatus parseTokenFalse(GC &gc);
    RuleStatus parseTokenFinally(GC &gc);
    RuleStatus parseTokenFor(GC &gc);
    RuleStatus parseTokenFunction(GC &gc);
    RuleStatus parseTokenIf(GC &gc);
    RuleStatus parseTokenImport(GC &gc);
    RuleStatus parseTokenIn(GC &gc);
    RuleStatus parseTokenInstanceOf(GC &gc);
    RuleStatus parseTokenLet(GC &gc);
    RuleStatus parseTokenNew(GC &gc);
    RuleStatus parseTokenNull(GC &gc);
    RuleStatus parseTokenReturn(GC &gc);
    RuleStatus parseTokenSuper(GC &gc);
    RuleStatus parseTokenSwitch(GC &gc);
    RuleStatus parseTokenThis(GC &gc);
    RuleStatus parseTokenThrow(GC &gc);
    RuleStatus parseTokenTrue(GC &gc);
    RuleStatus parseTokenTry(GC &gc);
    RuleStatus parseTokenTypeOf(GC &gc);
    RuleStatus parseTokenVar(GC &gc);
    RuleStatus parseTokenVoid(GC &gc);
    RuleStatus parseTokenWhile(GC &gc);
    RuleStatus parseTokenWith(GC &gc);
    RuleStatus parseTokenYield(GC &gc);
    RuleStatus parseTokenBooleanLiteral(GC &gc);
    RuleStatus parseTokenNumericLiteral(GC &gc);
    RuleStatus parseTokenStringLiteral(GC &gc);
    RuleStatus parseTokenRegularExpressionLiteral(GC &gc);
    RuleStatus parseTokenNoSubstitutionTemplate(GC &gc);
    RuleStatus parseTokenTemplateHead(GC &gc);
    RuleStatus parseTokenTemplateMiddle(GC &gc);
    RuleStatus parseTokenTemplateTail(GC &gc);
    RuleStatus parseTokenEndOfFile(GC &gc);
    RuleStatus parseOrInsertSemicolon(GC &gc,
                                      bool lineTerminatorAllowed,
                                      bool isTerminatingSemicolonInDoWhile);
    RuleStatus parseDirectivePrologue(GC &gc);
    template <bool hasYield>
    RuleStatus parseIdentifierReference(GC &gc);
    template <bool hasYield>
    RuleStatus parseBindingIdentifier(GC &gc);
    template <bool hasYield>
    RuleStatus parseLabelIdentifier(GC &gc);
    template <bool hasYield>
    RuleStatus parsePrimaryExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parseParenthesizedExpression(GC &gc);
    RuleStatus parseLiteral(GC &gc);
    template <bool hasYield>
    RuleStatus parseArrayLiteral(GC &gc);
    template <bool hasYield>
    RuleStatus parseElementList(GC &gc);
    RuleStatus parseElision(GC &gc);
    template <bool hasYield>
    RuleStatus parseSpreadElement(GC &gc);
    template <bool hasYield>
    RuleStatus parseObjectLiteral(GC &gc);
    template <bool hasYield>
    RuleStatus parsePropertyDefinitionList(GC &gc);
    template <bool hasYield>
    RuleStatus parsePropertyDefinition(GC &gc);
    template <bool hasYield>
    RuleStatus parsePropertyName(GC &gc);
    RuleStatus parseLiteralPropertyName(GC &gc);
    template <bool hasYield>
    RuleStatus parseComputedPropertyName(GC &gc);
    template <bool hasYield>
    RuleStatus parseCoverInitializedName(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseInitializer(GC &gc);
    template <bool hasYield>
    RuleStatus parseTemplateLiteral(GC &gc);
    template <bool hasYield>
    RuleStatus parseTemplateSpans(GC &gc);
    template <bool hasYield>
    RuleStatus parseTemplateMiddleList(GC &gc);
    template <bool hasYield>
    RuleStatus parseMemberExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parseSuperProperty(GC &gc);
    RuleStatus parseMetaProperty(GC &gc);
    RuleStatus parseNewTarget(GC &gc);
    template <bool hasYield>
    RuleStatus parseNewExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parseCallExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parseSuperCall(GC &gc);
    template <bool hasYield>
    RuleStatus parseArguments(GC &gc);
    template <bool hasYield>
    RuleStatus parseArgumentList(GC &gc);
    template <bool hasYield>
    RuleStatus parseLeftHandSideExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parsePostfixExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parseUnaryExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parseMultiplicativeExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parseAdditiveExpression(GC &gc);
    template <bool hasYield>
    RuleStatus parseShiftExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseRelationalExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseEqualityExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseBitwiseAndExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseBitwiseXorExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseBitwiseOrExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseLogicalAndExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseLogicalOrExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseConditionalExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseAssignmentExpression(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseExpression(GC &gc);
    template <bool hasYield, bool hasReturn>
    RuleStatus parseStatement(GC &gc);
    template <bool hasYield>
    RuleStatus parseDeclaration(GC &gc);
    template <bool hasYield, bool hasReturn>
    RuleStatus parseBlockStatement(GC &gc);
    template <bool hasYield, bool hasReturn>
    RuleStatus parseBlock(GC &gc);
    template <bool hasYield, bool hasReturn>
    RuleStatus parseStatementList(GC &gc);
    template <bool hasYield, bool hasReturn>
    RuleStatus parseStatementListItem(GC &gc);
    template <bool hasYield>
    RuleStatus parseVariableStatement(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseVariableDeclarationList(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseVariableDeclaration(GC &gc);
    RuleStatus parseEmptyStatement(GC &gc);
    template <bool hasYield>
    RuleStatus parseExpressionStatement(GC &gc);
    RuleStatus parseDebuggerStatement(GC &gc);
    template <bool hasIn, bool hasYield>
    RuleStatus parseArrowFunction(GC &gc);
    template <bool hasIn>
    RuleStatus parseYieldExpression(GC &gc);
    RuleStatus parseScript(GC &gc);
    void translateScript(GC &gc);
    RuleStatus parseScriptBody(GC &gc);
    void test(GC &gc);
    static value::ObjectHandle mainParseScript(SourceHandle source, GC &gc);
    static void mainTestParse(SourceHandle source, GC &gc);
};
}
}

#endif /* JAVASCRIPT_TASKLETS_PARSER_PARSER_IMP_H_ */
