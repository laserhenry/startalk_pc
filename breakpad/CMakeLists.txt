project(breakpad)

set(SOURCE_FILES )

include_directories(.)

if (APPLE)
    set(PLATFORM_SOURCE
            breakpad_googletest_includes.h
            client/mac/handler/exception_handler.h
            client/mac/crash_generation/crash_generation_client.h
            client/mac/crash_generation/crash_generation_server.h
            client/mac/crash_generation/client_info.h
            client/mac/handler/minidump_generator.h
            client/mac/handler/dynamic_images.h
            client/mac/handler/breakpad_nlist_64.h
            client/mac/handler/mach_vm_compat.h
            client/minidump_file_writer.h
            client/minidump_file_writer-inl.h
            common/mac/macho_utilities.h
            common/mac/byteswap.h
            common/mac/MachIPC.h
            common/mac/scoped_task_suspend-inl.h
            common/mac/file_id.h
            common/mac/macho_id.h
            common/mac/macho_walker.h
            common/mac/macho_utilities.h
            common/mac/bootstrap_compat.h
            common/mac/string_utilities.h
            common/linux/linux_libc_support.h
            common/string_conversion.h
            common/md5.h
            common/using_std_string.h
            common/convert_UTF.h
            google_breakpad/common/minidump_exception_mac.h
            google_breakpad/common/breakpad_types.h
            google_breakpad/common/minidump_format.h
            google_breakpad/common/minidump_size.h

            client/mac/handler/exception_handler.cc
            client/mac/crash_generation/crash_generation_client.cc
            client/mac/crash_generation/crash_generation_server.cc
            client/mac/handler/minidump_generator.cc
            client/mac/handler/dynamic_images.cc
            client/mac/handler/breakpad_nlist_64.cc
            client/minidump_file_writer.cc
            common/mac/macho_id.cc
            common/mac/macho_walker.cc
            common/mac/macho_utilities.cc
            common/mac/string_utilities.cc
            common/mac/file_id.cc
            common/mac/MachIPC.mm
            common/mac/bootstrap_compat.cc
            common/md5.cc
            common/string_conversion.cc
            common/convert_UTF.cc)

    link_libraries("-framework CoreFoundation")
    link_libraries("-framework CoreServices")

elseif (UNIX)
    set(PLATFORM_SOURCE
            client/linux/minidump_writer/cpu_set.h
            client/linux/minidump_writer/proc_cpuinfo_reader.h
            client/linux/handler/exception_handler.h
            client/linux/crash_generation/crash_generation_client.h
            client/linux/handler/minidump_descriptor.h
            client/linux/minidump_writer/minidump_writer.h
            client/linux/minidump_writer/line_reader.h
            client/linux/minidump_writer/linux_dumper.h
            client/linux/minidump_writer/linux_ptrace_dumper.h
            client/linux/minidump_writer/directory_reader.h
            client/linux/log/log.h
            client/linux/dump_writer_common/ucontext_reader.h
            client/linux/dump_writer_common/ucontext_reader.cc
            client/linux/dump_writer_common/thread_info.h
            client/linux/dump_writer_common/thread_info.cc
            client/linux/microdump_writer/microdump_writer.cc
            client/linux/microdump_writer/microdump_writer.h
            client/minidump_file_writer-inl.h
            client/minidump_file_writer.h
            common/linux/linux_libc_support.h
            common/linux/eintr_wrapper.h
            common/linux/ignore_ret.h
            common/linux/file_id.h
            common/linux/memory_mapped_file.h
            common/linux/safe_readlink.h
            common/linux/guid_creator.h
            common/linux/elfutils.h
            common/linux/elfutils-inl.h
            common/linux/elf_gnu_compat.h
            common/using_std_string.h
            common/basictypes.h
            common/memory_range.h
            common/string_conversion.h
            common/convert_UTF.h
            google_breakpad/common/minidump_format.h
            google_breakpad/common/minidump_size.h
            google_breakpad/common/breakpad_types.h
            common/scoped_ptr.h
            client/linux/crash_generation/crash_generation_client.cc
            client/linux/handler/exception_handler.cc
            client/linux/handler/minidump_descriptor.cc
            client/linux/minidump_writer/minidump_writer.cc
            client/linux/minidump_writer/linux_dumper.cc
            client/linux/minidump_writer/linux_ptrace_dumper.cc
            client/linux/log/log.cc
            client/minidump_file_writer.cc
            common/linux/linux_libc_support.cc
            common/linux/file_id.cc
            common/linux/memory_mapped_file.cc
            common/linux/safe_readlink.cc
            common/linux/guid_creator.cc
            common/linux/elfutils.cc
            common/string_conversion.cc
            common/convert_UTF.cc
            )
else ()
    set(PLATFORM_SOURCE
            common/windows/string_utils-inl.h
            common/windows/guid_string.h
            client/windows/handler/exception_handler.h
            client/windows/common/ipc_protocol.h
            google_breakpad/common/minidump_format.h
            google_breakpad/common/breakpad_types.h
            client/windows/crash_generation/crash_generation_client.h
            common/scoped_ptr.h
            client/windows/handler/exception_handler.cc
            common/windows/string_utils.cc
            common/windows/guid_string.cc
            client/windows/crash_generation/crash_generation_client.cc
            )
endif ()

set(LIBRARY_OUTPUT_PATH ../bin)
set(CMAKE_DEBUG_POSTFIX "d")

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${PLATFORM_SOURCE})