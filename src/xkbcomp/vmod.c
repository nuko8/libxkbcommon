/************************************************************
 * Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Silicon Graphics not be
 * used in advertising or publicity pertaining to distribution
 * of the software without specific prior written permission.
 * Silicon Graphics makes no representation about the suitability
 * of this software for any purpose. It is provided "as is"
 * without any express or implied warranty.
 *
 * SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 * GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 * THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 ********************************************************/

#include "xkbcomp-priv.h"
#include "text.h"
#include "expr.h"
#include "vmod.h"

bool
HandleVModDef(struct xkb_keymap *keymap, VModDef *stmt)
{
    xkb_mod_index_t i;
    const char *name;
    const struct xkb_vmod *vmod;
    struct xkb_vmod new;

    if (stmt->value)
        log_err(keymap->ctx,
                "Support for setting a value in a virtual_modifiers statement has been removed; "
                "Value ignored\n");

    name = xkb_atom_text(keymap->ctx, stmt->name);
    if (ModNameToIndex(name) != XKB_MOD_INVALID) {
        log_err(keymap->ctx,
                "Can't add a virtual modifier named \"%s\"; "
                "there is already a non-virtual modifier with this name! Ignored\n",
                name);
        return false;
    }

    darray_enumerate(i, vmod, keymap->vmods)
        if (vmod->name == stmt->name)
            return true;

    if (darray_size(keymap->vmods) >= XKB_MAX_VIRTUAL_MODS) {
        log_err(keymap->ctx,
                "Too many virtual modifiers defined (maximum %d)\n",
                XKB_MAX_VIRTUAL_MODS);
        return false;
    }

    new.name = stmt->name;
    new.mapping = 0;
    darray_append(keymap->vmods, new);
    return true;
}
