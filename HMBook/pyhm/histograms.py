# -*- coding: utf-8 -*-
"""Module with functions for dealing with histograms.

"""


def histo_square_line(x, y, borders=False):
    """Converts a histogram to a 'square-line' form.

    This line form provides a nice way to plot a histogram in a style analogue
    to the 'Histogram' style in hmview. After conversion just plot X and Y as a
    line.

    Parameters
    ----------
    x : list of floats
        x-coordinates.
    y : list of floats
        y-coordinates.
    borders : boolean, default False
        whether the x-coordinates are borders (True) or centers (False)

    Returns
    -------
    X : list of floats
        Converted x-coordinates.
    Y : list of floats
        Converted y-coordinates.

    Example
    -------
    >>> create_histo([1, 2, 3], [2, 4, 1])
    ([0.5, 1.5, 1.5, 2.5, 2.5, 3.5], [2, 2, 4, 4, 1, 1])

    >>> create_histo([0.5, 1.5, 2.5, 3.5], [2, 4, 1], borders=True)
    ([0.5, 1.5, 1.5, 2.5, 2.5, 3.5], [2, 2, 4, 4, 1, 1])

    """

    ln = len(y)

    X = [0.0 for i in range(2*ln)]
    Y = [0.0 for i in range(2*ln)]

    if not borders:
        xb = c2b(x)
    else:
        xb = x

    for i, yi in enumerate(y):
        X[2*i] = xb[i]
        X[2*i + 1] = xb[i + 1]
        Y[2*i] = yi
        Y[2*i + 1] = yi

    return X, Y


def cut_range(x, y, x_min, x_max, borders=False):
    """Returns histogram between the specified range.

    Paramters
    ---------
    x : list of floats
        x-coordinates
    y : list of floats
        y-coordinates
    x_min : float
        lower bound for returned histogram
    x_max : float
        upper bound for returned histogram
    borders : boolean, default False
        whether the x-coordinates are borders (True) or centers (False)

    Returns
    -------
    X : list of floats
        x-coordinates
    Y : list of floats
        y-coordinates

    Example
    -------
    >>> cut_range([0., 1., 2., 3.], [2, 1, 3, 4], 0.5, 2.)
    ([1.0, 2.0], [1, 3])

    >>> cut_range([2., 3., 4., 5., 6.], [2, 3, 1, 2], 2.5, 5., borders=True)
    ([3.0, 4.0, 5.0], [3, 1])

    """

    I = [i for i, xi in enumerate(x) if x_min <= xi <= x_max]
    if I == []:
        return [], []

    if not borders:
        return x[I[0]:I[-1]+1], y[I[0]:I[-1]+1]
    else:
        return x[I[0]:I[-1]+1], y[I[0]:I[-1]]


def b2c(x):
    """Converts the borders of the histogram to centers.

    Parameters
    ----------
    x : list of floats
        x-coordinates of borders

    Returns
    -------
    X : list of floats
        x-coordinates of centers

    Example
    -------
    >>> b2c([0., 1., 2., 3.])
    [0.5, 1.5, 2.5]

    """

    return [(xi + x[i+1])/2. for i, xi in enumerate(x[:-1])]


def c2b(x):
    """Converts the centers of the histogram to borders.

    Parameters
    ----------
    x : list of floats
        x-coordinates of centers

    Returns
    -------
    X : list of floats
        x-coordinates of borders

    Example
    -------
    >>> c2b([0.5, 1.5, 2.5])
    [0.0, 1.0, 2.0, 3.0]

    """

    dxh = (x[1] - x[0]) / 2.

    X = [xi - dxh for xi in x]
    X.append(x[-1] + dxh)

    return X
