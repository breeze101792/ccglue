/* 
 *   ccglue - cscope,ctags glue: Process ctags and cscope output to
 *        produce cross-reference table (for use with Vim CCTree plugin)
 *        and more ...
 *   Copyright (C) April, 2011,  Hari Rangarajan 
 *   Contact: hariranga@users.sourceforge.net

 *   "ccglue" is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with "ccglue".  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typedefs.h"
#include "seqfile.h"
#include "sym_mgr.h"
#include "cscoperdr.h"
#include "tagfilerdr.h"

RC_t process_files_with_reader (generic_db_rdr *reader, sym_table* a_sym_table, 
        const char* db_files)
{
    uchar		 line[1024];
    int			 idx = 0;
    int                  lines = 0;
    char                *db_file_name;
    char                 db_filenames_var[512];
    seq_file            *db_file;

    /* need to copy before we try tokenizing */
    strcpy(db_filenames_var, db_files);
    db_file_name = strtok(db_filenames_var, ",");

    db_file = new seq_file;

    while (db_file_name != NULL) {
        if (db_file->reader_create(db_file_name) == RC_SUCCESS) {
            while (db_file->read_till_newline(line, 
                        sizeof(line)) != RC_FAILURE) {
                reader->process_line(a_sym_table, line);
                lines++;
            }
            db_file->close();
        } else {
            return RC_FAILURE;
        }
        db_file_name = strtok(NULL, ",");
    }
    delete db_file;

    return RC_SUCCESS;
}
    

RC_t process_cscope_files_to_build_sym_table (sym_table* a_sym_table, 
        char* cscope_files)
{
    cscope_db_rdr *rdr = new cscope_db_rdr;
    rdr->set_scan_action(ACTION_LOAD_SYMS);
    process_files_with_reader(rdr, a_sym_table, cscope_files);
    delete rdr;
}

RC_t process_cscope_files_to_build_xrefs (sym_table* a_sym_table,
        char* cscope_files)
{
    cscope_db_rdr *rdr = new cscope_db_rdr;
    rdr->set_scan_action(ACTION_XREF_SYMS);
    process_files_with_reader(rdr, a_sym_table, cscope_files);
    delete rdr;
}

