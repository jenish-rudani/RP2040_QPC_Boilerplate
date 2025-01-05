# Set up QP environment
set(QP_DEFAULT_BASE "/Users/atom/Documents/BB/Pico/QPC_DEFAULT_BASE" CACHE QP_DEFAULT_BASE "QP default base directory")
if (NOT QP_DEFAULT_BASE)
    message(FATAL_ERROR "QP_DEFAULT_BASE not defined.")
endif()
set(QPC "${QP_DEFAULT_BASE}/qpc")

if (QSPY)
    message("Using QP/Spy Software Tracing.")
    add_compile_options(-DQ_SPY)
endif()

if(QP_RTEF STREQUAL "QPC")
    message("Using QP/C framework.")
    set(QP ${QPC})
    set(QF_SOURCES
        ${QP}/src/qf/qep_hsm.c
        ${QP}/src/qf/qep_msm.c
        ${QP}/src/qf/qf_act.c
        ${QP}/src/qf/qf_actq.c
        ${QP}/src/qf/qf_defer.c
        ${QP}/src/qf/qf_dyn.c
        ${QP}/src/qf/qf_mem.c
        ${QP}/src/qf/qf_ps.c
        ${QP}/src/qf/qf_qact.c
        ${QP}/src/qf/qf_qeq.c
        ${QP}/src/qf/qf_qmact.c
        ${QP}/src/qf/qf_time.c
    )
    if(QSPY)
        set(QF_SOURCES
            ${QF_SOURCES}
            ${QP}/src/qs/qs.c
            ${QP}/src/qs/qs_64bit.c
            ${QP}/src/qs/qs_fp.c
            ${QP}/src/qs/qs_rx.c
            ${QP}/src/qs/qstamp.c
            #${QP}/src/qs/qutest.c
            )
    endif()

    if(QP_KERNEL STREQUAL "QK")
        message("Using preemptive, non-blocking kernel (QK).")
        set(QP_SOURCES
            ${QF_SOURCES}
            ${QP}/src/qk/qk.c
            ${QP}/ports/arm-cm/qk/gnu/qk_port.c
            ${QP}/ports/arm-cm/qk/gnu/qp_port.h
            ${QP}/ports/arm-cm/config/qp_config.h
        )
        if(QSPY)
            set(QP_SOURCES
                ${QP_SOURCES}
                ${QP}/ports/arm-cm/qk/gnu/qs_port.h
            )
        endif()
        set(QP_INCLUDE ${QP}/include ${QP}//ports/arm-cm/qk/gnu ${QP}//ports/arm-cm/qk/config)
    elseif(QP_KERNEL STREQUAL "QV")
        message("Using non-preemptive, cooperative kernel (QV).")
        set(QP_SOURCES
            ${QF_SOURCES}
            ${QP}/src/qv/qv.c
            ${QP}/ports/arm-cm/qv/gnu/qv_port.c
            ${QP}/ports/arm-cm/qv/gnu/qp_port.h
            ${QP}/ports/arm-cm/qv/config/qp_config.h
        )
        if(QSPY)
            set(QP_SOURCES
                ${QP_SOURCES}
                ${QP}/ports/arm-cm/qv/gnu/qs_port.h
            )
        endif()
        set(QP_INCLUDE ${QP}/include ${QP}//ports/arm-cm/qv/gnu ${QP}//ports/arm-cm/qk/config)
    else()
        message(FATAL_ERROR "QP_KERNEL not defined. Must be <QK|QV>")
    endif()
else()
    message(FATAL_ERROR "QP framework unknown. Must be <QPC|QPCPP>")
endif()
