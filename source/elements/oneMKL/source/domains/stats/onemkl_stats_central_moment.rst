.. _onemkl_stats_central_moment:

central_moment
==============

Entry point to compute central moments up to the 4th order.

.. _onemkl_stats_central_moment_description:

.. rubric:: Description and Assumptions

oneapi::mkl::stats::central_moment function is used to compute array of central moments up to the 4th order (central moments for each dataset's dimension).

:ref:`onemkl_stats_central_moment` supports the following precisions for data:

    .. list-table::
        :header-rows: 1

        * - T
        * - ``float``
        * - ``double``


.. _onemkl_stats_central_moment_buffer:

central_moment (Buffer version)
-------------------------------

.. rubric:: Syntax

.. cpp:function:: template<oneapi::mkl::stats::method Method = oneapi::mkl::stats::method::fast, typename Type, \
                   oneapi::mkl::stats::layout ObservationsLayout> \
                   void oneapi::mkl::stats::central_moment(sycl::queue& queue, \
                   const oneapi::mkl::stats::dataset<sycl::buffer<Type, 1>, ObservationsLayout>& data, \
                   sycl::buffer<Type, 1> central_moment_2, \
                   sycl::buffer<Type, 1> central_moment_3 = {0}, \
                   sycl::buffer<Type, 1> central_moment_4 = {0});

.. container:: section

    .. rubric:: Template Parameters

    Method
        Method which is used for estimate computation. The specific values are as follows:

        *  ``oneapi::mkl::stats::method::fast``
        *  ``oneapi::mkl::stats::method::one_pass``

    Type
        Data precision.

    ObservationsLayout
        Data layout. The specific values are described in :ref:`onemkl_stats_dataset`.

.. container:: section

    .. rubric:: Input Parameters

    queue
        The queue where the routine should be executed.

    data
        Dataset which is used for computation.

.. container:: section

    .. rubric:: Output Parameters

    central_moment_2
        sycl::buffer array of 2nd order central moment values.

    central_moment_3
        Optional parameter. sycl::buffer array of 3rd order central moment values.

    central_moment_4
        Optional parameter. sycl::buffer array of 4th order central moment values.


.. _onemkl_stats_central_moment_usm:

central_moment (USM version)
----------------------------

.. rubric:: Syntax


.. cpp:function:: template<oneapi::mkl::stats::method Method = oneapi::mkl::stats::method::fast, typename Type, \
                  oneapi::mkl::stats::layout ObservationsLayout> \
                  sycl::event oneapi::mkl::stats::central_moment(sycl::queue& queue, \
                  const dataset<Type*, ObservationsLayout>& data, Type* central_moment_2, \
                  Type* central_moment_3 = nullptr, Type* central_moment_4 = nullptr, \
                  const sycl::vector_class<sycl::event> &dependencies = {});

.. container:: section

    .. rubric:: Template Parameters

    Method
        Method which is used for estimate computation. The specific values are as follows:

        *  ``oneapi::mkl::stats::method::fast``
        *  ``oneapi::mkl::stats::method::one_pass``

    Type
        Data precision.

    ObservationsLayout
        Data layout. The specific values are described in :ref:`onemkl_stats_dataset`.

.. container:: section

    .. rubric:: Input Parameters

    queue
        The queue where the routine should be executed.

    data
        Dataset which is used for computation.

    dependencies
        Optional parameter. List of events to wait for before starting computation, if any.


.. container:: section

    .. rubric:: Output Parameters

    central_moment_2
        Pointer to the array of the 2nd order central moment values.

    central_moment_3
        Optional parameter. Pointer to the array of the 3rd order central moment values.

    central_moment_4
        Optional parameter. Pointer to the array of the 2nd order central moment values.


.. container:: section

    .. rubric:: Return Value

    Output event to wait on to ensure computation is complete.


**Parent topic:** :ref:`onemkl_stats_routines`

