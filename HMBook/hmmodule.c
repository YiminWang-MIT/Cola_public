#include <Python.h>

#include <HMBook/hmbook.h>

#include <math.h>

// Generates list of data points for 1D histogram.
void generate_list_1D(HIST *id, PyObject *list, PyObject *errors, PyObject *borders)
{
  // PyObject_IsTrue evaluates to 1 if true and 0 if false.
  // There is 1 more border than centers.
  PyObject *x = PyList_New(id->nx + PyObject_IsTrue(borders));
  PyObject *y = PyList_New(id->nx);
  int i;
  // Distance between two bins.
  double dx = (id->maxx - id->minx)/(FLOAT)id->nx;

  if (PyObject_IsTrue(borders))
    // Values are on the edges of bins.
    for (i=0; i<id->nx + 1; i++) PyList_SetItem(x, i, Py_BuildValue("d", id->minx + dx*i));
  else
    // Values are on the center of bins.
    for (i=0; i<id->nx; i++) PyList_SetItem(x, i, Py_BuildValue("d", id->minx + dx*(i + 0.5)));
  for (i=0; i<id->nx; i++) PyList_SetItem(y, i, Py_BuildValue("d", id->data[i]));

  PyList_Append(list, x);
  PyList_Append(list, y);
  Py_DECREF(x);
  Py_DECREF(y);

  if (PyObject_IsTrue(errors))
  {
    PyObject *dy = PyList_New(id->nx);

    // Histograms from Simul++ have an error array, but ones form Cola++ do not.
    if(id->error)
      for (i=0; i<id->nx; i++) PyList_SetItem(dy, i, Py_BuildValue("d", id->error[i]));
    else
      for (i=0; i<id->nx; i++) PyList_SetItem(dy, i, Py_BuildValue("d", sqrt(id->data[i])));

    PyList_Append(list, dy);
    Py_DECREF(dy);
  }
}

// Generates list of data points for 2D histogram.
void generate_list_2D(HIST *id, PyObject *list, PyObject *errors, PyObject *borders)
{
  // PyObject_IsTrue evaluates to 1 if true and 0 if false.
  // There is 1 more border than centers.
  PyObject *x = PyList_New(id->nx + PyObject_IsTrue(borders));
  PyObject *y = PyList_New(id->ny + PyObject_IsTrue(borders));
  PyObject *z = PyList_New(id->nx*id->ny);
  int i;
  // Distance between two bins.
  double dx = (id->maxx - id->minx)/(FLOAT)id->nx;
  double dy = (id->maxy - id->miny)/(FLOAT)id->ny;

  if (PyObject_IsTrue(borders))
  {
    // Values are on the edges of bins.
    for (i=0; i<id->nx + 1; i++) PyList_SetItem(x, i, Py_BuildValue("d", id->minx + dx*i));
    for (i=0; i<id->ny + 1; i++) PyList_SetItem(y, i, Py_BuildValue("d", id->miny + dy*i));
  }
  else
  {
    // Values are on the center of bins.
    for (i=0; i<id->nx; i++) PyList_SetItem(x, i, Py_BuildValue("d", id->minx + dx*(i + 0.5)));
    for (i=0; i<id->ny; i++) PyList_SetItem(y, i, Py_BuildValue("d", id->miny + dy*(i + 0.5)));
  }
  for (i=0; i<id->nx*id->ny; i++) PyList_SetItem(z, i, Py_BuildValue("d", id->data[i]));

  PyList_Append(list, x);
  PyList_Append(list, y);
  PyList_Append(list, z);
  Py_DECREF(x);
  Py_DECREF(y);
  Py_DECREF(z);

  if (PyObject_IsTrue(errors))
  {
    PyObject *dz = PyList_New(id->nx*id->ny);

    // Histograms from Simul++ have an error array, but ones form Cola++ do not.
    if(id->error)
      for (i=0; i<id->nx*id->ny; i++) PyList_SetItem(dz, i, Py_BuildValue("d", id->error[i]));
    else
      for (i=0; i<id->nx*id->ny; i++) PyList_SetItem(dz, i, Py_BuildValue("d", sqrt(id->data[i])));

    PyList_Append(list, dz);
    Py_DECREF(dz);
  }
}

// Gets axes labels and units.
void generate_labels(HIST *hi, PyObject *list)
{
  PyObject *labels = PyList_New(0);
  PyObject *units = PyList_New(0);

  PyList_Append(labels, Py_BuildValue("s", hi->labelx));
  PyList_Append(units, Py_BuildValue("s", hi->unitx));
  PyList_Append(labels, Py_BuildValue("s", hi->labely));
  PyList_Append(units, Py_BuildValue("s", hi->unity));
  // Checks if histogram is 2D.
  if (hi->ny != 0)
  {
    PyList_Append(labels, Py_BuildValue("s", hi->labelz));
    PyList_Append(units, Py_BuildValue("s", hi->unitz));
  }

  PyList_Append(list, labels);
  PyList_Append(list, units);
  Py_DECREF(labels);
  Py_DECREF(units);
}


PyDoc_STRVAR(gethisto__doc__,
"pyhm.gethisto(filepath, histogram_title, errors=False, labels=False, borders=False)\n\n"
"Parameters:\n"
"  filepath: string\n"
"  histogram_title: string\n"
"  errors: boolean, default False\n"
"    get count uncertainty for each bin\n"
"  labels: boolean, default False\n"
"    get axis labels and units\n"
"  borders: boolean, default False\n"
"    bins positions are given on bin edges instead of bin center\n\n"
"Returns:\n"
"  histogram: list of lists\n"
"    x: list of floats\n"
"    y: list of floats\n"
"    z: list of floats (if 2D histogram)\n"
"    errors: list of floats (if errors=True)\n"
"    labels: list of strings (if labels=True)\n"
"    units: list of strings (if labels=True)");

// Gets a histogram from a file.
static PyObject *
pyhm_gethisto(PyObject *self, PyObject *args, PyObject *keywds)
{
  char *filename, *title;
  // Get any type of variable and then get its true value with PyObject_IsTrue().
  PyObject *errors = Py_False;
  PyObject *labels = Py_False;
  PyObject *borders = Py_False;
  static char *kwlist[] = {"filename", "title", "errors", "labels", "borders", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, keywds, "ss|OOO", kwlist,
                                   &filename, &title, &errors, &labels, &borders))
    return NULL;

  HIST *hi = HMScanFile(filename, title);
  if (!hi)
  {
    printf("Could not open his file!\n");
    Py_RETURN_NONE;
  }

  PyObject *list = PyList_New(0);
  // If histogram is 1D...
  if (hi->ny == 0) generate_list_1D(hi, list, errors, borders);
  else generate_list_2D(hi, list, errors, borders);
  if (PyObject_IsTrue(labels)) generate_labels(hi, list);

  HMDelete(hi);
  return list;
}

// legacy
// gethisto1d('hisfile', 'histogram')
static PyObject *
pyhm_gethisto1d(PyObject *self, PyObject *args)
{
  char *filename, *title;
  PyObject *errors = Py_False;
  PyObject *borders = Py_False;

  if (!PyArg_ParseTuple(args, "ss", &filename, &title))
    return NULL;
  HIST *hi = HMScanFile(filename, title);
  if (!hi) {
    printf("could not open his file\n");
    Py_RETURN_NONE;
  }

  PyObject *list = PyList_New(0);
  generate_list_1D(hi, list, errors, borders);

  Py_DECREF(borders);
  HMDelete(hi);
  return list;
}

// legacy
// gethisto2d('hisfile', 'histogram')
static PyObject *
pyhm_gethisto2d(PyObject *self, PyObject *args)
{
  char *filename, *title;
  PyObject *errors = Py_False;
  PyObject *borders = Py_False;

  if (!PyArg_ParseTuple(args, "ss", &filename, &title))
    return NULL;
  HIST *hi = HMScanFile(filename, title);
  if (!hi) {
    printf("could not open his file\n");
    Py_RETURN_NONE;
  }

  PyObject *list = PyList_New(0);
  generate_list_2D(hi, list, errors, borders);

  Py_DECREF(borders);
  HMDelete(hi);
  return list;
}


PyDoc_STRVAR(readall__doc__,
"pyhm.readall(filepath, errors=False, labels=False, borders=False)\n\n"
"Parameters:\n"
"  filepath: string\n"
"  errors: boolean, default False\n"
"    get count uncertainty for each bin\n"
"  labels: boolean, default False\n"
"    get axis labels and units\n"
"  borders: boolean, default False\n"
"    bins positions are given on bin edges instead of bin center\n\n"
"Returns:\n"
"  histograms: list of histograms\n"
"    histogram: list of list\n"
"      histogram_title: string\n"
"      x: list of floats\n"
"      y: list of floats\n"
"      z: list of floats (if 2D histogram)\n"
"      errors: list of floats (if errors=True)\n"
"      labels: list of strings (if labels=True)\n"
"      units: list of strings (if labels=True)");

// Gets all histograms from a file.
static PyObject *
pyhm_readall(PyObject *self, PyObject *args, PyObject *keywds)
{
  char *filename;
  // Get any type of variable and then get its true value with PyObject_IsTrue().
  PyObject *errors = Py_False;
  PyObject *labels = Py_False;
  PyObject *borders = Py_False;
  static char *kwlist[] = {"filename", "errors", "labels", "borders", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, keywds, "s|OOO", kwlist,
                                   &filename, &errors, &labels, &borders))
    return NULL;

  FILE *fp = HMOpenHisFile(filename);
  HIST *id;
  // Read first histogram.
  id = HMReadIsolated(fp);
  if (!id) Py_RETURN_NONE;

  PyObject *all = PyList_New(0);
  // Loop over all histograms in a file.
  while(id)
  {
    PyObject *hist = PyList_New(0);
    PyList_Append(hist, Py_BuildValue("s", id->title));

    // If histogram is 1D...
    if (id->ny == 0) generate_list_1D(id, hist, errors, borders);
    else generate_list_2D(id, hist, errors, borders);
	  if (PyObject_IsTrue(labels)) generate_labels(id, hist);

	  PyList_Append(all, hist);
	  Py_DECREF(hist);

      // Free all allocated arrays.
	  if (id->data)    free(id->data);
	  if (id->datasqr) free(id->datasqr);
	  if (id->nbin)    free(id->nbin);
	  if (id->error)   free(id->error);
	  if (id->xvalues) free(id->xvalues);
    free(id);

	// Read next histogram in a file.
    id = HMReadIsolated(fp);
  }

  pclose(fp);
  return all;
}


PyDoc_STRVAR(readbynum__doc__,
"pyhm.readbynum(filepath, histnums, errors=False, labels=False, borders=False)\n\n"
"Parameters:\n"
"  filepath: string\n"
"  histnums: list of integers\n"
"    list of serial numbers of histograms to be returned\n"
"    Histogram numbering is the same as the order the histogram titles are\n"
"    returned from pyhm.getnames. The list should be sorted (ascending) and\n"
"    within [0, #histograms).\n"
"    Be warned: there is no checking for input validity yet!\n"
"  errors: boolean, default False\n"
"    get count uncertainty for each bin\n"
"  labels: boolean, default False\n"
"    get axis labels and units\n"
"  borders: boolean, default False\n"
"    bins positions are given on bin edges instead of bin center\n\n"
"Returns:\n"
"  histograms: list of histograms\n"
"    histogram: list of list\n"
"      histogram_title: string\n"
"      x: list of floats\n"
"      y: list of floats\n"
"      z: list of floats (if 2D histogram)\n"
"      errors: list of floats (if errors=True)\n"
"      labels: list of strings (if labels=True)\n"
"      units: list of strings (if labels=True)");

// Gets histograms by their position in a file.
static PyObject *
pyhm_readbynum(PyObject *self, PyObject *args, PyObject *keywds)
{
  char *filename;
  PyObject *histnums;
  // Get any type of variable and then get its true value with PyObject_IsTrue().
  PyObject *errors = Py_False;
  PyObject *labels = Py_False;
  PyObject *borders = Py_False;
  static char *kwlist[] = {"filename", "histnums", "errors", "labels", "borders", NULL};

  if (!PyArg_ParseTupleAndKeywords(args, keywds, "sO!|OOO", kwlist,
                                   &filename, &PyList_Type, &histnums, &errors, &labels, &borders))
    return NULL;

  Py_ssize_t i, j;
  const Py_ssize_t len = PyList_Size(histnums);
  long nums[len];
  // Parse list of wanted histograms.
  for (i=0; i<len; i++) nums[i] = PyLong_AsLong(PyList_GetItem(histnums, i));

  FILE *fp = HMOpenHisFile(filename);
  HIST *id;
  // Read first histogram.
  id = HMReadIsolated(fp);
  if (!id) Py_RETURN_NONE;

  PyObject *hists = PyList_New(0);
  i = 0;  // Index of current histogram in a file.
  j = 0;  // Index of current element in a array of wanted histograms.
  // Loop over all histograms in a file.
  while(id)
  {
    // If current histogram is wanted, build it.
    if (i == nums[j])
    {
      PyObject *hist = PyList_New(0);
      PyList_Append(hist, Py_BuildValue("s", id->title));

      // If histogram is 1D...
      if (id->ny==0) generate_list_1D(id, hist, errors, borders);
      else generate_list_2D(id, hist, errors, borders);
      if (PyObject_IsTrue(labels)) generate_labels(id, hist);

      PyList_Append(hists, hist);
      Py_DECREF(hist);

      j++;
    }

    // Free all allocated arrays.
    if (id->data) free(id->data);
    if (id->datasqr) free(id->datasqr);
    if (id->nbin) free(id->nbin);
    if (id->error) free(id->error);
    if (id->xvalues) free(id->xvalues);
    free(id);

    i++;
	// Read next histogram in a file.
    id = HMReadIsolated(fp);
  }

  return hists;
}


PyDoc_STRVAR(getnames__doc__,
"pyhm.getnames(filepath)\n\n"
"Parameters:\n"
"  filepath: string\n\n"
"Returns:\n"
"  histogram_names: list of strings");

// Gets names of all histograms in a file.
static PyObject *
pyhm_getnames(PyObject *self, PyObject *args)
{
  char *filename;

  if (!PyArg_ParseTuple(args, "s", &filename))
    return NULL;
  FILE *fp = HMOpenHisFile(filename);
  HIST *id;
  // Read first histogram.
  id = HMReadIsolated(fp);
  if (!id) Py_RETURN_NONE;

  PyObject *names = PyList_New(0);
  // Loop over all histograms in a file.
  while(id) {
    PyList_Append(names, Py_BuildValue("s", id->title));

    // Free all allocated arrays.
    if (id->data)    free(id->data);
    if (id->datasqr) free(id->datasqr);
    if (id->nbin)    free(id->nbin);
    if (id->error)   free(id->error);
    if (id->xvalues) free(id->xvalues);
    free(id);

    // Read next histogram in a file.
    id = HMReadIsolated(fp);
  }
  pclose(fp);
  return names;
}

// legacy
// gethistinfo('hisfile', 'histogram')
static PyObject *
pyhm_gethistinfo(PyObject *self, PyObject *args)
{
  char *filename, *title;

  if (!PyArg_ParseTuple(args, "ss", &filename, &title)) return NULL;
  HIST *hi = HMScanFile(filename, title);
  if (!hi) Py_RETURN_NONE;

  PyObject *info = PyList_New(0);

  PyList_Append(info, Py_BuildValue("s", hi->title));
  PyList_Append(info, Py_BuildValue("s", hi->labelx));
  PyList_Append(info, Py_BuildValue("s", hi->unitx));
  PyList_Append(info, Py_BuildValue("s", hi->labely));
  PyList_Append(info, Py_BuildValue("s", hi->unity));
  if (hi->ny != 0)
  {
    PyList_Append(info, Py_BuildValue("s", hi->labelz));
    PyList_Append(info, Py_BuildValue("s", hi->unitz));
  }

  HMDelete(hi);

  return info;
}

struct module_state {
    PyObject *error;
};

#if PY_MAJOR_VERSION >= 3
#define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#else
#define GETSTATE(m) (&_state)
static struct module_state _state;
#endif

static PyObject *
pyhm_error_out(PyObject *m) {
    struct module_state *st = GETSTATE(m);
    PyErr_SetString(st->error, "something bad happened");
    return NULL;
}

// List of all functions in a Python module.
static PyMethodDef pyhmMethods[] = {
  { "gethistinfo", (PyCFunction)pyhm_gethistinfo, METH_VARARGS,
    "returns info on histograms as (h_title, ax1_title, ax1_unit, ...)" },
  { "gethisto", (PyCFunction)pyhm_gethisto, METH_VARARGS | METH_KEYWORDS,
    gethisto__doc__},
  { "gethisto1d", (PyCFunction)pyhm_gethisto1d, METH_VARARGS,
    "return a histogram" },
  { "gethisto2d", (PyCFunction)pyhm_gethisto2d, METH_VARARGS,
    "return a 2d histogram" },
  { "getnames", (PyCFunction)pyhm_getnames, METH_VARARGS,
    getnames__doc__ },
  { "readall", (PyCFunction)pyhm_readall, METH_VARARGS | METH_KEYWORDS,
    readall__doc__ },
  { "readbynum", (PyCFunction)pyhm_readbynum, METH_VARARGS | METH_KEYWORDS,
    readbynum__doc__ },
  { "error_out", (PyCFunction)pyhm_error_out, METH_NOARGS,
    "handle errors"},
  { NULL, NULL, 0, NULL }
};

#if PY_MAJOR_VERSION >= 3
static int pyhm_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int pyhm_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}

static struct PyModuleDef moduledef = {
        PyModuleDef_HEAD_INIT,
        "core",
        NULL,
        sizeof(struct module_state),
        pyhmMethods,
        NULL,
        pyhm_traverse,
        pyhm_clear,
        NULL
};

#define INITERROR return NULL

PyObject *
PyInit_core(void)

#else
#define INITERROR return

void
initcore(void)
#endif
{
#if PY_MAJOR_VERSION >= 3
    PyObject *module = PyModule_Create(&moduledef);
#else
    PyObject *module = Py_InitModule("core", pyhmMethods);
#endif

    if (module == NULL)
        INITERROR;
    struct module_state *st = GETSTATE(module);

    st->error = PyErr_NewException("core.Error", NULL, NULL);
    if (st->error == NULL) {
        Py_DECREF(module);
        INITERROR;
    }

#if PY_MAJOR_VERSION >= 3
    return module;
#endif
}

int
main(int argc, char *argv[])
{
  /* Pass argv[0] to the Python interpreter */
  Py_SetProgramName(
#if PY_MAJOR_VERSION >= 3
		    (wchar_t *)
#endif
		    argv[0]);

  /* Initialize the Python interpreter. Required. */
  Py_Initialize();
  return 0;
}
