/*
    ������ v3.0 ��� ������� 'Advanced-SA-NickName' v4.0!
    ������, ���������� ��������� ��� � ������ ����, � ����� ����������� ������� �������� ����.
		1)����������� ������� ���� � ����� 'pawno/include'
		2)����������� �� ���� ����� �������� � ����� ����� ����� '#include <a_samp>'
			��������� '#include <ASAN>'
		3)��� �������� ���� �� ����������, ����������� ������� IsValidNickName(name[])
			����������: true - ��� �������, false - � ���� ����������� �������.
    ���������: [KrYpToDeN]
*/

#if !defined _samp_included
	#error Include 'ASAN.inc' should be loaded after 'a_samp.inc'!
#endif

#if !defined _string_included
	#error Include 'ASAN.inc' should be loaded after 'string.inc'!
#endif

#if defined _ASAN_included
	#endinput
#endif

#define _ASAN_included
#pragma library ASAN

native ASAN_GetPlayerName(name[]);
native IsValidNickName(name[]);

stock _GetPlayerName(playerid, name[], length)
{
    new return_value = GetPlayerName(playerid, name, length);
    ASAN_GetPlayerName(name);
	return return_value;
}

#if defined _ALS_GetPlayerName
    #undef GetPlayerName
#else
    #define _ALS_GetPlayerName
#endif
#define GetPlayerName _GetPlayerName

/*
    ������ v3.0 ��� ������� 'Advanced-SA-NickName' v4.0!
    ������, ���������� ��������� ��� � ������ ����, � ����� ����������� ������� �������� ����.
		1)����������� ������� ���� � ����� 'pawno/include'
		2)����������� �� ���� ����� �������� � ����� ����� ����� '#include <a_samp>'
			��������� '#include <ASAN>'
		3)��� �������� ���� �� ����������, ����������� ������� IsValidNickName(name[])
			����������: true - ��� �������, false - � ���� ����������� �������.
    ���������: [KrYpToDeN]
*/