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

if(process.argv.length != 3 || process.argv[2] == '' || process.argv[2].substr(0, 1) == '-')
{
    console.log("usage: ./create_category_function.js <unicode category>");
    console.log("generates a C++ function that recognizes the Unicode category <unicode category>.");
    process.exit(1);
}

var generateCategory = process.argv[2];

/*

mapfile -t unicode_data < UCD/UnicodeData.txt

function write_progress()
{
    if [[ "$1" ]]; then
        printf "loading... %2i%%\r" "$1"
    else
        printf "done.               \n"
    fi
}

character_category=()
character_ranges=()
codepoints_done=0
defined_codepoint_count=260253
for line in "${unicode_data[@]}"; do
    if [[ "$line" =~ ^([0-9A-Fa-f]+)';'([^;]*)';'([^;]*)';' ]]; then
        code="0x${BASH_REMATCH[1]}"
        category="${BASH_REMATCH[3]}"
        name="${BASH_REMATCH[2]}"
        if [[ "$name" =~ ^'<'[^'<>,']+', '('First'|'Last')'>'$ ]]; then
            character_ranges+=("$line")
        else
            character_category[code]="$category"
            if ((++codepoints_done % 0x400 == 0)); then
                write_progress "$((codepoints_done * 100 / defined_codepoint_count))"
            fi
        fi
    else
        echo
        echo "bad line: $line" >&2
        exit 1
    fi
done
for((i=0;i<${#character_ranges[@]};i+=2)); do
    if [[ "${character_ranges[i]}" =~ ^([0-9A-Fa-f]+)';'([^;]*)';'([^;]*)';' ]]; then
        start_code="0x${BASH_REMATCH[1]}"
        start_category="${BASH_REMATCH[3]}"
        start_name="${BASH_REMATCH[2]}"
        if [[ "$start_name" =~ ^'<'([^<>,]+)', First>'$ ]]; then
            start_name="${BASH_REMATCH[1]}"
        else
            echo
            echo "bad range start: ${character_ranges[i]}" >&2
            exit 1
        fi
    else
        echo
        echo "bad line: ${character_ranges[i]}" >&2
        exit 1
    fi
    if [[ "${character_ranges[i + 1]}" =~ ^([0-9A-Fa-f]+)';'([^;]*)';'([^;]*)';' ]]; then
        end_code="0x${BASH_REMATCH[1]}"
        end_category="${BASH_REMATCH[3]}"
        end_name="${BASH_REMATCH[2]}"
        if [[ "$end_name" =~ ^'<'([^<>,]+)', Last>'$ ]]; then
            end_name="${BASH_REMATCH[1]}"
        else
            echo
            echo "bad range end: ${character_ranges[i + 1]}" >&2
            exit 1
        fi
    else
        echo
        echo "bad line: ${character_ranges[i + 1]}" >&2
        exit 1
    fi
    if [[ "$start_code" -gt "$end_code" || "$start_category" != "$end_category" || "$start_name" != "$end_name" ]]; then
        printf "\nbad range:\n%s\n%s\n" "${character_ranges[i]}" "${character_ranges[i + 1]}" >&2
        exit 1
    fi
    for((code=start_code;code<=end_code;code++)); do
        character_category[code]="$start_category"
        if ((++codepoints_done % 0x1000 == 0)); then
            write_progress "$((codepoints_done * 100 / defined_codepoint_count))"
        fi
    done
done
write_progress

function even_range_start()
{
    printf "even start: %06X\n" "$1"
}

function odd_range_start()
{
    printf "odd start: %06X\n" "$1"
}

function even_range_end()
{
    printf "even end: %06X\n" "$1"
}

function odd_range_end()
{
    printf "odd end: %06X\n" "$1"
}

even_matches=0
odd_matches=0
for((codepoint=0;codepoint<0x110000 + 2;codepoint+=2)); do
    if [[ "${character_category[codepoint]}" == "$generate_category" ]]; then
        if ((!even_matches)); then
            even_matches=1
            even_range_start "$codepoint"
        fi
    else
        if ((even_matches)); then
            even_matches=0
            even_range_end "$((codepoint - 2))"
        fi
    fi
    if [[ "${character_category[codepoint + 1]}" == "$generate_category" ]]; then
        if ((!odd_matches)); then
            odd_matches=1
            odd_range_start "$((codepoint + 1))"
        fi
    else
        if ((odd_matches)); then
            odd_matches=0
            odd_range_end "$((codepoint - 1))"
        fi
    fi
done
*/
