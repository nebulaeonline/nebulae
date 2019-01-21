<?php
// Copyright (c) 2018-2019 Nebulae Foundation. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

// Takes a VirtualBox memory dump (as an .elf file), and given the offset
// of the memory dump, the cr3 value and the location of the new page
// tables, will dump all of them from the memory dump.
//
// These items can be found like so:
// Start VirtualBox to be able to create a memory dump:
//   VirtualBox --dbg --startvm nebulae
// Create the memory dump at the appropriate time:
//   VBoxManage debugvm "nebulae" dumpvmcore --filename nebdump.elf
// Find the offset of the elf64 section that contains the memory dump: 
//   objdump -h nebdump.elf | egrep -w "(Idx|load1)"
// CR3 and the new page table location is provided by nebulae and/or
// Vbox.log, depending on what you're doing.  You could also just dump
// it to screen before panicing

require_once("uint64.php");

if ($argc != 5) {
    fwrite(STDOUT, "USAGE: {$argv[0]} <dumpfile> <memory_section_offset> <cr3> <new_page_table_location>\n\n");
    exit();
}

if (!file_exists($argv[1])) {
    fwrite(STDOUT, "File {$argv[1]} not found\n");
    exit();
}

if (!is_numeric($argv[2]) || !is_numeric($argv[3]) || !is_numeric($argv[4])) {
    fwrite(STDOUT, "Please specify integer arguments for memory_section_offset, cr3, and new_page_table_location\n");
    exit();
}

if (is_null(($elf = fopen($argv[1], "r")))) {
    fwrite(STDOUT, "Unable to open file: {$argv[1]}\n");
    exit();
}

// Assign the start of the physical memory dump
// within the elf file
$memory_start_offset = intval($argv[2]);

// 
SeekToMemStart();


// This function sets the file pointer back to the 
// offset of the start of the physical memory dump 
function SeekToMemStart() {
    global $memory_start_offset, $elf;

    fseek($elf, $memory_start_offset, SEEK_SET);
}

?>
