import os
from sw import *

DIR_NEWDOC = os.path.join(app_exe_dir(), 'data', 'newdoc')

class Command:
    def menu(self):
        if not os.path.isdir(DIR_NEWDOC): return
        files = os.listdir(DIR_NEWDOC)
        if not files:
            msg_status('No files in data/newdoc')
            return

        files = [(item, lexer_proc(LEXER_DETECT, item)) for item in files]

        lexers = sorted(list(set([item[1] for item in files if item[1]])))
        if not lexers: return

        res = dlg_menu(MENU_SIMPLE, '', '\n'.join(lexers))
        if res is None: return

        lexer = lexers[res]
        files = sorted([item[0] for item in files if item[1]==lexer])
        if not files: return

        if len(files)==1:
            fn = files[0]
        else:
            res = dlg_menu(MENU_SIMPLE, '', '\n'.join(files))
            if res is None: return
            fn = files[res]

        lexer = lexer_proc(LEXER_DETECT, fn)

        fn = os.path.join(DIR_NEWDOC, fn)
        file_open('')
        ed.set_text_all(open(fn).read())

        if lexer:
            ed.set_prop(PROP_LEXER_FILE, lexer)

        msg_status('New file from "%s"' % os.path.basename(fn))
