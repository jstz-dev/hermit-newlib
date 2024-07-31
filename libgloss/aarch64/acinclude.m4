AARCH64_OBJTYPE=
case "${target}" in
  *-*-elf | *-*-hermit)
	AARCH64_OBJTYPE=elf-
	;;
esac
AC_SUBST(AARCH64_OBJTYPE)
