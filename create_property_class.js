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

for(var codePoint = 0, lastMatched = false; codePoint < characterProperty.length; codePoint++)
{
    if(characterProperty[codePoint] === generateProperty)
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
var code = 'UnicodeProperty' + generateProperty + ':char ';
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
