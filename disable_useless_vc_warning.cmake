
if(MSVC)
  # Disable following warnings
  # C4819: Code page mismatched warning
  # SECURE WARNINGS: Common warnings appear when using POSIX or libc.
  add_definitions ( -wd4819 -wd4250 -wd4800 -wd4345
                    -D_C_SECURE_NO_WARNINGS
                    -D_SCL_SECURE_NO_WARNINGS )
endif()
