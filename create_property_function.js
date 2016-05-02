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
    console.log("usage: ./create_property_function.js <unicode property>");
    console.log("generates a C++ function that recognizes the Unicode property <unicode property>.");
    process.exit(1);
}

var generateProperty = process.argv[2];

function writeProgress(progress)
{
    if(progress == null)
        console.log("done.              ");
    else
        process.stdout.write("loading... " + (' ' + Math.floor(progress).toString()).substr(-2) + "%\r");
}

function loadUnicodeData()
{
    var unicodeData = fs.readFileSync("UCD/PropList.txt", {encoding:"utf-8"}).split('\n');
    var characterProperty = [];
    console.assert(unicodeData.length > 0);
    if(unicodeData[unicodeData.length - 1] === '')
    {
        unicodeData.pop();
    }
    for(var i = 0; i < unicodeData.length; i++)
    {
        var line = unicodeData[i].split('#')[0].trim();
        if(line === '')
            continue;
        line = line.split(';').map(function(v){return v.trim();});
        var property = line[1];
        console.assert(line.length == 2);
        if(property === generateProperty)
        {
            var rangeMatch = line[0].match(/^([a-fA-F0-9]+)\.\.([a-fA-F0-9]+)$/);
            if(rangeMatch)
            {
                var start = +('0x' + rangeMatch[1]);
                var end = +('0x' + rangeMatch[2]);
                for(var j = start; j <= end; j++)
                {
                    characterProperty[j] = property;
                }
            }
            else
            {
                characterProperty[+('0x' + line[0])] = property;
            }
        }
    }
    return characterProperty;
}

var characterProperty = loadUnicodeData();

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
    includes: function(codepoint)
    {
        codepoint >>>= 0;
        return codepoint >= this.start && codepoint <= this.end && (codepoint & 1 ? this.includeEven : this.includeOdd);
    },
    toString: function(varName)
    {
        if(!varName)
            varName = 'codepoint'
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

for(var codepoint = 0, lastMatched = false; codepoint < characterProperty.length; codepoint++)
{
    if(characterProperty[codepoint] === generateProperty)
    {
        if(!lastMatched)
        {
            matches.push(new Match(codepoint, codepoint, true, true));
        }
        else
        {
            matches[matches.length - 1].end = codepoint;
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
console.log('constexpr bool property' + generateProperty + '(std::uint32_t codepoint) noexcept\n{\n    return ' + code.replace(/ \|\| /g, ' ||\n        ') + ';\n}\n');
var testFn = new Function('codepoint', 'codepoint >>>= 0; return ' + code.replace(/(0x[A-F0-9]+)UL?/g, '$1'));
for(var codepoint = 0; codepoint < characterProperty.length; codepoint++)
{
    if(testFn(codepoint) !== (characterProperty[codepoint] === generateProperty))
    {
        throw new Error('mismatch at ' + Match.getHex(codepoint));
    }
}

