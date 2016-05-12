#!/usr/bin/nodejs
// Copyright (C) 2012-2016 Jacob R. Lifshay
// This file is part of Voxels.
//
// Voxels is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Voxels is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Voxels; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.
//
//

var fs = require("fs");

if(process.argv.length != 3 || process.argv[2] == '' || process.argv[2].substr(0, 1) == '-')
{
    console.log("usage: ./create_category_function.js <unicode category>");
    console.log("generates a C++ function that recognizes the Unicode category <unicode category>.");
    process.exit(1);
}

var generateCategory = process.argv[2];

function writeProgress(progress)
{
    if(progress == null)
        console.log("done.              ");
    else
        process.stdout.write("loading... " + (' ' + Math.floor(progress).toString()).substr(-2) + "%\r");
}

function loadUnicodeData()
{
    var unicodeData = fs.readFileSync("UCD/UnicodeData.txt", {encoding:"utf-8"}).split('\n');
    var characterCategory = [];
    console.assert(unicodeData.length > 0);
    if(unicodeData[unicodeData.length - 1] === '')
    {
        unicodeData.pop();
    }
    for(var i = 0; i < unicodeData.length; i++)
    {
        var line = unicodeData[i].split(';');
        var category = line[2];
        console.assert(line.length == 15);
        if(/^<[^>]+, First>$/.test(line[1]))
        {
            i++;
            console.assert(i < unicodeData.length);
            var line2 = unicodeData[i].split(';');
            console.assert(line2.length == 15);
            console.assert(/^<[^>]+, Last>$/.test(line2[1]));
            var start = +('0x' + line[0]);
            var end = +('0x' + line2[0]);
            for(var j = start; j <= end; j++)
            {
                characterCategory[j] = category;
            }
        }
        else
        {
            characterCategory[+('0x' + line[0])] = category;
        }
    }
    return characterCategory;
}

var characterCategory = loadUnicodeData();

function Match(start, end, includeEven, includeOdd)
{
    this.start = start >>> 0;
    this.end = end >>> 0;
    this.includeEven = !!includeEven;
    this.includeOdd = !!includeOdd;
}

Match.getHex = function(v)
{
    v >>>= 0;
    var s = '0x' + v.toString(16).toUpperCase() + 'U';
    if(v > 0xFFFF)
        s += 'L';
    return s;
}

Match.prototype =
{
    isSingle: function()
    {
        return this.start == this.end;
    },
    includes: function(codePoint)
    {
        codePoint >>>= 0;
        return codePoint >= this.start && codePoint <= this.end && (codePoint & 1 ? this.includeEven : this.includeOdd);
    },
    toString: function(varName)
    {
        if(!varName)
            varName = 'codePoint'
        if(this.start == this.end)
        {
            return varName + ' == ' + Match.getHex(this.start);
        }
        if(this.includeEven && this.includeOdd)
        {
            if(this.start == this.end - 1)
            {
                return varName + ' == ' + Match.getHex(this.start) + ' || ' + varName + ' == ' + Match.getHex(this.end);
            }
            return '(' + varName + ' >= ' + Match.getHex(this.start) + ' && ' + varName + ' <= ' + Match.getHex(this.end) + ')';
        }
        if(this.includeEven)
        {
            return '(' + varName + ' >= ' + Match.getHex(this.start) + ' && ' + varName + ' <= ' + Match.getHex(this.end) + ' && ' + varName + ' % 2 == 0)';
        }
        return '(' + varName + ' >= ' + Match.getHex(this.start) + ' && ' + varName + ' <= ' + Match.getHex(this.end) + ' && ' + varName + ' % 2 != 0)';
    },
};

var matches = [];

for(var codePoint = 0, lastMatched = false; codePoint < characterCategory.length; codePoint++)
{
    if(characterCategory[codePoint] === generateCategory)
    {
        if(!lastMatched)
        {
            matches.push(new Match(codePoint, codePoint, true, true));
        }
        else
        {
            matches[matches.length - 1].end = codePoint;
        }
        lastMatched = true;
    }
    else
    {
        lastMatched = false;
    }
}

var oldMatches = matches;
matches = [];

for(var i = 0; i < oldMatches.length; i++)
{
    var match = oldMatches[i];
    if(matches.length === 0 || match.end != match.start)
    {
        matches.push(match);
        continue;
    }
    var lastMatch = matches[matches.length - 1];
    var canCombine = match.start == lastMatch.end + 2;
    if(canCombine)
    {
        if(match.start & 1)
            canCombine = lastMatch.includeOdd && (lastMatch.start == lastMatch.end || !lastMatch.includeEven);
        else
            canCombine = lastMatch.includeEven && (lastMatch.start == lastMatch.end || !lastMatch.includeOdd);
    }
    if(canCombine)
    {
        lastMatch.end = match.start;
        if(match.start & 1)
            lastMatch.includeEven = false;
        else
            lastMatch.includeOdd = false;
    }
    else
    {
        matches.push(match);
    }
}

var code = matches.join(' || ');
console.log('constexpr bool category' + generateCategory + '(std::uint32_t codePoint) noexcept\n{\n    return ' + code.replace(/ \|\| /g, ' ||\n        ') + ';\n}\n');
var testFn = new Function('codePoint', 'codePoint >>>= 0; return ' + code.replace(/(0x[A-F0-9]+)UL?/g, '$1'));
for(var codePoint = 0; codePoint < characterCategory.length; codePoint++)
{
    if(testFn(codePoint) !== (characterCategory[codePoint] === generateCategory))
    {
        throw new Error('mismatch at ' + Match.getHex(codePoint));
    }
}

