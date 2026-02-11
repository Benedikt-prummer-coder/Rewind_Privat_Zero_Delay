// utils.cpp
#include "utils.h"
#include <Windows.h>

bool IsAdmin()
{
    BOOL admin = FALSE;
    SID_IDENTIFIER_AUTHORITY nt = SECURITY_NT_AUTHORITY;
    PSID sid;
    AllocateAndInitializeSid(&nt,2,SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,0,0,0,0,0,0,&sid);
    CheckTokenMembership(nullptr,sid,&admin);
    FreeSid(sid);
    return admin;
}
