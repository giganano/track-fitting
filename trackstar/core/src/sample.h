/*
This file is part of the TrackStar package.
Copyright (C) 2023 James W. Johnson (giganano9@gmail.com)
License: MIT License. See LICENSE in top-level directory
at: https://github.com/giganano/TrackStar.git
*/

#ifndef SAMPLE_H
#define SAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "matrix.h"
#include "datum.h"

typedef struct sample {

	/*
	.. cpp:type:: SAMPLE

	A collection of data vectors in some observed space. Some vectors may be
	missing measurements for certain quantities.

	Attributes
	----------
	data : ``DATUM **``
		The collection of vectors themselves, each stored as a pointer to a
		``DATUM`` object.
	n_vectors : ``unsigned long``
		The number of vectors in ``data`` (i.e., the sample size).
	*/

	DATUM **data;
	unsigned long n_vectors;

} SAMPLE;

/*
.. cpp:function:: extern SAMPLE *sample_initialize(unsigned long sample_size);

Allocate memory for and return a pointer to a ``SAMPLE`` object.
Automatically allocates memory for the appropriate number of ``DATUM`` objects,
but does not allocate memory for each individual ``DATUM`` stored. That is,
after calling this function, the user will have to call ``datum_initialize``
the appropriate number of times to copy their data into this object.

Parameters
----------
sample_size : ``unsigned long``
	The number of data vectors in the sample. This will be copied over to the
	attribute ``n_vectors`` of the newly constructed object.

Returns
-------
s : ``SAMPLE *``
	The newly constructed sample, ready to have its vectors initialized.
*/
extern SAMPLE *sample_initialize(void);

/*
.. cpp:function:: extern void sample_free(SAMPLE *s);

Free up the memory associated with a ``SAMPLE`` object that is user-facing.

Parameters
----------
s : ``SAMPLE *``
	The sample to be freed.

Notes
-----
In practice, this function should only be called upon exiting the python
interpreter, or when a user calls ``del`` on their ``SAMPLE`` object.

The important difference between this function and ``sample_free_everything``
is that this function does not free *every* block of memory stored by a
``SAMPLE`` object. Doing so causes memory errors because Cython automatically
calls the necessary ``__dealloc__`` functions that do free up the required
blocks of memory. Namely, this function does not call ``datum_free`` for
each individual ``DATUM`` stored by ``s``, because Cython calls
``datum.__dealloc__`` with the same addresses as ``sample.data``.
*/
extern void sample_free(SAMPLE *s);

/*
.. cpp:function:: extern void sample_free_everything(SAMPLE *s);

Free up the memory associated with a ``SAMPLE`` object that is *not*
user-facing.

Notes
-----
In practice, this function should only be called for ``SAMPLE``
objects created in TrackStar's C library or cdef'ed instances created in Cython
that are not returned to the user.

.. seealso::

	See "Notes" under function ``sample_free`` for details on the differences
	between these two functions.
*/
extern void sample_free_everything(SAMPLE *s);

/*
.. cpp:function:: extern void sample_add_datum(SAMPLE *s, DATUM *d);

Add a new data vector to the sample.

Parameters
----------
s : ``SAMPLE *``
	The sample object, which potentially already contains other data
	vectors already added to the sample.
d : ``DATUM *``
	The vector to be included in this sample.
*/
extern void sample_add_datum(SAMPLE *s, DATUM *d);

/*
.. cpp:function:: extern SAMPLE *sample_specific_quantities(SAMPLE s,
	char **labels, unsigned short n_labels);

Obtain a pointer to a ``SAMPLE`` object containing the relevant information
for only *some* of the quantities stored in a given ``SAMPLE``.

Parameters
----------
s : ``SAMPLE``
	The input sample of data vectors.
labels : ``char **``
	The column labels to pull from the sample object.
n_labels : ``unsigned short``
	The number of elements in ``label``.

Returns
-------
sub : ``SAMPLE *``
	A new ``SAMPLE`` object, containing only the labels, vector components, and
	covariance matrix entries associated with particular measurements.
*/
extern SAMPLE *sample_specific_quantities(SAMPLE s, char **labels,
	const unsigned short n_labels);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SAMPLE_H */
