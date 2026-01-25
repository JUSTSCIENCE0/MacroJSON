// Copyright (c) 2026, Yakov Usoltsev
// Email: yakovmen62@gmail.com
//
// License: MIT

// struct generation macros
#ifdef MJSON_OBJECT_BEGIN
#  undef MJSON_OBJECT_BEGIN
#endif

#ifdef MJSON_FIELD
#  undef MJSON_FIELD
#endif

#ifdef MJSON_OBJECT_END
#  undef MJSON_OBJECT_END
#endif

// enum generation macros
#ifdef MJSON_ENUM_BEGIN
#  undef MJSON_ENUM_BEGIN
#endif

#ifdef MJSON_ENUM_UNIT
#  undef MJSON_ENUM_UNIT
#endif

#ifdef MJSON_ENUM_VALUE
#  undef MJSON_ENUM_VALUE
#endif

#ifdef MJSON_ENUM_ALIAS
#  undef MJSON_ENUM_ALIAS
#endif

#ifdef MJSON_ENUM_END
#  undef MJSON_ENUM_END
#endif