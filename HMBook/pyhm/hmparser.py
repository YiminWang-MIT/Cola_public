# -*- coding: utf-8 -*-
"""Module with functions for parsing hm style strings.

"""


def hm2latex(text):
    """Converts hm style writing to LaTeX style.

    hm2latex(text)

    Parameters
    ----------
    text : string
        Text to convert.

    Returns
    -------
    text_conv : string
        Converted text.

    Example
    -------
    >>> hm2latex('F_[Q]!')
    'F_\\mathrm{{\\Theta}}'

    """

    # Transform string to list, so we can loop over and change elements.
    atxt = list(text)

    # Greek alphabet tranformations.
    greek_ch = {'a': r'\alpha', 'b': r'\beta', 'g': r'\gamma', 'd': r'\delta',
                'e': r'\epsilon', 'z': r'\zeta', 'h': r'\eta', 'q': r'\theta',
                'i': r'\iota', 'k': r'\kappa', 'l': r'\lambda', 'm': r'\mu',
                'n': r'\nu', 'x': r'\xi', 'o': r'\omicron', 'p': r'\pi',
                'r': r'\rho', 's': r'\sigma', 't': r'\tau', 'u': r'\upsilon',
                'f': r'\phi', 'c': r'\chi', 'y': r'\psi', 'w': r'\omega',

                'A': r'\Alpha', 'B': r'\Beta', 'G': r'\Gamma', 'D': r'\Delta',
                'E': r'\Epsilon', 'Z': r'\Zeta', 'H': r'\Eta', 'Q': r'\Theta',
                'I': r'\Iota', 'K': r'\Kappa', 'L': r'\Lambda', 'M': r'\Mu',
                'N': r'\Nu', 'X': r'\Xi', 'O': r'\Omicron', 'P': r'\Pi',
                'R': r'\Rho', 'S': r'\Sigma', 'T': r'\Tau', 'U': r'\Upsilon',
                'F': r'\Phi', 'C': r'\Chi', 'Y': r'\Psi', 'W': r'\Omega'}

    # Characters that need escaping.
    escape_c = {'%': r'\%',
                '#': r'\#'}
    # Escape sequences to convert.
    escape_s = {'\\253': r'\leftrightarrow'}

    i = 0
    greek = False
    while i < len(atxt):
        # Greek mode
        if atxt[i] == '[':
            atxt[i] = '{'
            greek = True
        elif atxt[i] == ']':
            atxt[i] = '}'
            greek = False
        elif greek and atxt[i] in greek_ch:
            atxt[i] = greek_ch[atxt[i]]
        elif greek and text[i:i+4] in escape_s:
            atxt[i:i+4] = [escape_s[text[i:i+4]], '', '', '']

        # sub/super-script
        elif atxt[i] == '_':
            atxt[i] = r'_\mathrm{'
        elif atxt[i] == '^':
            atxt[i] = r'^\mathrm{'
        elif atxt[i] == '!':
            atxt[i] = '}'

        # other
        elif atxt[i] in escape_c:
            atxt[i] = escape_c[atxt[i]]
        elif atxt[i] == '\xb0':
            atxt[i] = r'^{\circ}'
        elif atxt[i] == r' ':
            atxt[i] = r'\,'

        i += 1

    # Transform list back to string and return it.
    return r''.join(atxt)


def latex_label(label, unit=None):
    """Converts hm style axis label (and units) into latex style axis label
    (with units).

    latex_label(label, unit=None)

    Parameters
    ----------
    label : string
        Axis label to convert.
    unit : string, default None
        Axis units to convert.

    Returns
    -------
    label_ax : string
        Converted axis label (with units).

    Example
    -------
    >>> latex_label('F_x!', unit='N')
    'F_\\mathrm{x} \\quad [\\mathrm{N}]'

    """

    if not unit:
        return hm2latex(label)
    else:
        return(r''.join([hm2latex(label), r' \quad [\mathrm{',
                         hm2latex(unit), '}]']))
