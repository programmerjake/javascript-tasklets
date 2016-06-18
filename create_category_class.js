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

function Match(start, end)
{
    this.start = start >>> 0;
    this.end = end >>> 0;
}

Match.escapeChar = function(v)
{
    v >>>= 0;
    if(v < 0x7F && v >= 0x20)
    {
        var char = String.fromCharCode(v);
        if('\\[]^:-'.indexOf(char) === -1)
            return char;
    }
    if(v <= 0x7F)
    {
        return '\\x' + ('00' + v.toString(16).toUpperCase()).slice(-2);
    }
    if(v <= 0xFFFF)
    {
        return '\\u' + ('0000' + v.toString(16).toUpperCase()).slice(-4);
    }
    return '\\U' + ('00000000' + v.toString(16).toUpperCase()).slice(-8);
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
        return codePoint >= this.start && codePoint <= this.end;
    },
    toString: function()
    {
        if(this.start == this.end)
        {
            return Match.escapeChar(this.start);
        }
        return Match.escapeChar(this.start) + '-' + Match.escapeChar(this.end);
    },
};

var matches = [];

for(var codePoint = 0, lastMatched = false; codePoint < characterCategory.length; codePoint++)
{
    if(characterCategory[codePoint] === generateCategory)
    {
        if(!lastMatched)
        {
            matches.push(new Match(codePoint, codePoint));
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

var line = '';
var code = 'UnicodeCategory' + generateCategory + ':char ';
var indent = '';
while(indent.length < code.length)
    indent += ' ';
code += '= [';
var variableNumber = 0;
for(var i = 0; i < matches.length; i++)
{
    if(line.length > 100)
    {
        code += line + "]:char" + variableNumber + " {$$ = char" + variableNumber++ + ";}\n" + indent + "/ [";
        line = "";
    }
    line += matches[i];
}
code += line + "]:char" + variableNumber + " {$$ = char" + variableNumber + ";};";

console.log(code);
