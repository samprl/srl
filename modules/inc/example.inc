cmd:buyfuel(playerid,params[])
{
	if(IsPlayerConnected(playerid))
	{
		if(PlayerInfo[playerid][pPayDayHad] < 4) return SendClientMessage(playerid, COLOR_GREY, " Вы должны пробыть в игре ещё 3 часа.");
		if(fuelbuy == 1) return SendClientMessage(playerid,COLOR_WHITE,"Администрация временно отключила возможность покупать дома.");
		if(PlayerInfo[playerid][pPbiskey] != 255 || PlayerInfo[playerid][pPsbiskey] != 255)  return SendClientMessage(playerid, COLOR_WHITE, "У вас уже есть бизнес, введите /sellbiz чтобы его продать.");
		if(PlayerInfo[playerid][pBizLic] == 0)  return	SendClientMessage(playerid, COLOR_WHITE, "У Вас нет лицензии на бизнес");
		for(new b = 0; b < sizeof(SBizzInfo); b++)
		{
			if(PlayerToPoint(10.0, playerid, SBizzInfo[b][sEntranceX], SBizzInfo[b][sEntranceY], SBizzInfo[b][sEntranceZ]) && SBizzInfo[b][sOwned] == 0)
			{
				if(PlayerInfo[playerid][pBank] < SBizzInfo[b][sBuyPrice]) return SendClientMessage(playerid,COLOR_GREY,"У вас недостаточно денег в банке.");
				//
				PlayerInfo[playerid][pBank] -= SBizzInfo[b][sBuyPrice];//Списали деньги с БАНКОВСКОГО счета
				PlayerInfo[playerid][pPsbiskey] = b;//ID бизнеса в статистику
				PlayerInfo[playerid][pJob] = 0;//Увольли игрока с работы
				strmid(SBizzInfo[b][sOwner], PlayerInfo[playerid][pNames], 0, strlen(PlayerInfo[playerid][pNames]), 255);//Задаем имя владельца
				SBizzInfo[b][sOwned] = 1;//Заправка куплена
				SBizzInfo[b][sLocked] = 0;//Заправка открыта
				SBizzInfo[b][sTill] = 100000;//100000 литров бензина при покупке
				SBizzInfo[b][sPrice] = 1000;//Цена за 1 литр
				//SBizzInfo[b][sPriceBenz] = 1111;//хз для чего
				//SBizzInfo[b][sBenz] = 2222;//хз для чего
				//
				SendClientMessage(playerid, COLOR_GREEN, "Поздравляем, вы купили заправку.");
				SendClientMessage(playerid, COLOR_GREEN, "Для уравления заправкой используйте команду /fpanel");
				format(mysql_query, sizeof(mysql_query), "UPDATE `sbizz` SET `sOwner` = '%s', `sOwned` = '1' WHERE sID = '%i'",PlayerInfo[playerid][pNames],b);
				mysql_function_query(connects, mysql_query, false, "", "");
				OnPlayerUpdateRL(playerid);
				SaveSBizz(b);
				UpdateSBizz();
				return true;
			}
		}
	}
	return true;
}
cmd:asellfuel(playerid,params[])
{
	if(IsPlayerConnected(playerid))
	{
		extract params -> new b;else
			SCM(playerid, COLOR_GRAD1, "Введите: /asellfuel [BIZID]");
		if(FD1 || FD2 || FD3)
		{
			if(PlayerInfo[playerid][pAdmInections] > 12)
			{
				format(mysql_query, sizeof(mysql_query), "UPDATE `accounts` SET `pPsbiskey` = '255' WHERE `Name` = '%s'", SBizzInfo[b][sOwner]);
				mysql_function_query(connects, mysql_query, false, "", "");
				//
				SBizzInfo[b][sBuyPrice] = 1000000;
				SBizzInfo[b][sOwned] = 0;
				SBizzInfo[b][sLocked] = 1;
				SBizzInfo[b][sPrice] = 2500;
				SBizzInfo[b][sBenz] = 2500;
				SBizzInfo[b][sPriceBenz] = 2500;
				SBizzInfo[b][sTill] = 100000;
				//
				strmid(SBizzInfo[b][sOwner], "No-one", 0, strlen("No-one"), 255);
				format(mysql_query, sizeof(mysql_query), "UPDATE `sbizz` SET `sOwner` = 'No-one' WHERE sID = %i",b);
				mysql_function_query(connects, mysql_query, false, "", "");
				SaveSBizz(b);
				UpdateSBizz();
				return true;
			}
		}
	}
	return true;	
}
cmd:sellfuel(playerid,params[])
{
	if(IsPlayerConnected(playerid))
	{
		if(PlayerInfo[playerid][pPsbiskey] == 255) return SendClientMessage(playerid,COLOR_GREY,"У вас нету заправки.");
		for(new b = 0; b < sizeof(SBizzInfo); b++)
		{
			if(PlayerToPoint(10.0, playerid, SBizzInfo[b][sEntranceX], SBizzInfo[b][sEntranceY], SBizzInfo[b][sEntranceZ]))
			{
				if(PlayerInfo[playerid][pPsbiskey] != b) return SendClientMessage(playerid,COLOR_GREY,"Вы не владелец этой заправки.");
				new nalogs = SBizzInfo[b][sBuyPrice]/10;
				PlayerInfo[playerid][pBank] += (SBizzInfo[b][sBuyPrice]-nalogs);
				PlayerInfo[playerid][pPsbiskey] = 255;
				//
				SBizzInfo[b][sBuyPrice] = 1000000;
				SBizzInfo[b][sOwned] = 0;
				SBizzInfo[b][sLocked] = 1;
				SBizzInfo[b][sPrice] = 2500;
				SBizzInfo[b][sBenz] = 2500;
				SBizzInfo[b][sPriceBenz] = 2500;
				SBizzInfo[b][sTill] = 100000;
				//
				strmid(SBizzInfo[b][sOwner], "No-one", 0, strlen("No-one"), 255);
				format(mysql_query, sizeof(mysql_query), "UPDATE `sbizz` SET `sOwner` = 'No-one' WHERE sID = %i",b);
				mysql_function_query(connects, mysql_query, false, "", "");
				OnPlayerUpdateRL(playerid);
				SaveSBizz(b);
				UpdateSBizz();
				return true;
			}
		}
	}
	return true;
}
cmd:setfuel(playerid,params[])
{
	if(PlayerInfo[playerid][pAdmInections] > 12)
	{
		extract params -> new b;else
			SCM(playerid, COLOR_GRAD1, "Введите: /setfuel [ID]");
		new Float:PosX,Float:PosY,Float:PosZ;
		GetPlayerPos(playerid,PosX,PosY,PosZ);
		SBizzInfo[b][sEntranceX] = PosX;
		SBizzInfo[b][sEntranceY] = PosY;
		SBizzInfo[b][sEntranceZ] = PosZ;
		format(mysql_query, sizeof(mysql_query), "UPDATE `sbizz` SET `sEntranceX` = '%f',`sEntranceY` = '%f',`sEntranceZ` = '%f' WHERE sID = %i",PosX,PosY,PosZ,b);
		mysql_function_query(connects, mysql_query, false, "", "");
		UpdateSBizz();
	}
	return true;
}
cmd:gps(playerid,params[])
{
	SPD(playerid, 9624, DIALOG_STYLE_LIST, "Global Positioning System", "{ffd700}-{FFFFFF} Важные места\n{ffd700}-{FFFFFF} Базы организаций\n{ffd700}-{FFFFFF} Автосалоны и мастерские\n{ffd700}-{FFFFFF} Работы\n{ffd700}-{FFFFFF} Бизнесы", "Выбрать", "Отмена");
	DisablePlayerCheckpoint(playerid);
	PicCP[playerid] = 0;
	return true;
}
cmd:enter(palyerid,params[])
{
	for(new i = 0; i < sizeof(HouseInfo); i++)
	{
		if(PlayerToPoint(2, playerid, HouseInfo[i][hEntrancex], HouseInfo[i][hEntrancey], HouseInfo[i][hEntrancez]))
		{
			if(PlayerInfo[playerid][pPhousekey] == i || HouseInfo[i][hLock] == 0)
			{
				SetPlayerInterior(playerid, HouseInfo[i][hInt]);
				SetPlayerVirtualWorld(playerid, i+50);
				SetPPos(playerid, HouseInfo[i][hExitx], HouseInfo[i][hExity], HouseInfo[i][hExitz]);
				PlayerInfo[playerid][pInt] = HouseInfo[i][hInt];
				return true;
			}
			else return GameTextForPlayer(playerid, "~r~Closed", 5000, 1);
		}
	}
	return true;	
}
cmd:exit(playerid,params[])
{
	for(new i = 0; i < sizeof(HouseInfo); i++)
	{
		if (PlayerToPoint(3, playerid,HouseInfo[i][hExitx], HouseInfo[i][hExity], HouseInfo[i][hExitz]))
		{
			if (GetPlayerVirtualWorld(playerid) == i+50)
			{
				SetPlayerInterior(playerid,0);
				SetPlayerVirtualWorld(playerid,0);
				SetPPos(playerid,HouseInfo[i][hEntrancex],HouseInfo[i][hEntrancey],HouseInfo[i][hEntrancez]);
				PlayerInfo[playerid][pInt] = 0;
				PlayerInfo[playerid][pLocal] = 255;
			}
		}
	}
	return true;	
}
cmd:directory(playerid,params[])
{
	SPD(playerid,5454,DIALOG_STYLE_LIST,"Справочник","[0] Адвокаты\n[1] Крупье\n[2] Лидеры организаций\n[3] Инструкторы\n[4] Механики\n[5] Медики\n[6] Развозчики продуктов\n[7] Тренеры\n[8] Развозчик строй-материалов","Выбрать","Отмена");
	return true;
}
alias:directory("dir");
cmd:c(playerid,params[])
{
	format(stringer, sizeof(stringer), "%s достаёт мобильник", PlayerInfo[playerid][pNames]);
	ProxDetector(30.0, playerid, stringer, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
	if(PlayerCuffed[playerid] == 0) {    SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USECELLPHONE); }
	SPD(playerid, 9956, 2, "Выберите сервис", "[0] Вызвать Таксиста \n[1] Вызвать Медицинскую помощь\n[2] Вызвать Полицию", "Далее", "Выйти");
	return true;
}
alias:c("call");
//----------------------------------[MainMenu /mm]-----------------------------------------------
cmd:mm(playerid,params[])
{
	new lvlexp[80];
	new level = PlayerInfo[playerid][pLevel];
	new nxtlevel = PlayerInfo[playerid][pLevel]+1;
	new exp = PlayerInfo[playerid][pExp];
	new expamount = nxtlevel*levelexp;
	format(lvlexp,70,"{AEFFFF}Игровой уровень: %d | Очки опыта: %d из %d",level,exp,expamount);
	SPD(playerid, 9623, DIALOG_STYLE_LIST, lvlexp, "{00FF00}<> {80FF00} Статистика персонажа »\n{00FF00}<> {80FF00} Улучшения »\n{00FF00}<> {80FF00} Настройки »\n{00FF00}<> {80FF00} Команды сервера »\n{00FF00}<> {80FF00} Обратиться к администрации »\n{00FF00}<> {80FF00} Задать вопрос хелперам »\n{00FF00}<> {80FF00} Доступ к игре »\n{00FF00}<> {80FF00} Донат »\n{00FF00}<> {80FF00} Навыки оружие(Скиллы) »\n{00FF00}<> {ff0000} Дополнительно(реальный донат) »", "Выбрать", "Отмена");
	showhelp[playerid]= false;
	showhelp2[playerid]= false;
	return true;
}
cmd:switchskin(playerid,params[])
{
	if(IsAGang(playerid))
	{
		if(forma[playerid] != true) return  SendClientMessage(playerid, COLOR_GRAD1, "У вас нет формы армейца");
		SetPlayerSkin(playerid,287);
		SetPlayerColor(playerid, COLOR_GREEN);
		forma[playerid] = false;
		proverkaforma[playerid] = true;
	}
	return true;
}
cmd:carm(playerid,params[])
{
	if(PlayerInfo[playerid][pMRInections] == 19 || proverkaforma[playerid] == true) return SPD(playerid, 9653, DIALOG_STYLE_LIST, "Развозка Оружия", "- Загрузиться\n- Разгрузиться\n- Разгрузить на склад Армии SF\n- Разгрузка в LSPD\n- Разгрузка в FBI\n- Разгрузка в RCPD\n- Разгрузка Бандам\n- Разгрузка в LVPD", "Выбрать", "Отмена");
}
cmd:givephone(playerid,params[])
{
	if(ISAMAFIA(playerid))
	{
		if(PlayerInfo[playerid][pRKInections] < 3)return SendClientMessage(playerid, COLOR_GREY,"Вам недоступна эта функция!");
		extract params -> new player:ID;else
			SCM(playerid, COLOR_GRAD1, "Введите: /givephone [ID]");
		if (!ProxDetectorS(8.0, playerid, ID)) return SendClientMessage(playerid, COLOR_GREY, "Человек не рядом с вами!");
		if(ID == playerid) return SendClientMessage(playerid, COLOR_GREY, "Нельзя вернуть телефон самому себе!");
		format(stringer, sizeof(stringer), "%s вернул вам телефон", PlayerInfo[playerid][pNames]);
		SendClientMessage(ID, 0x6ab1ffaa, stringer);
		takephone[ID] = false;
		format(stringer, sizeof(stringer), "%s вернул телефон %s 'y", PlayerInfo[playerid][pNames] ,PlayerInfo[ID][pNames]);
		ProxDetector(30.0, playerid, stringer, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
	}
	else SendClientMessage(playerid, COLOR_GREY, "Вы не член мафии!");
	return true;
}
cmd:takephone(playerid,params[])
{
	if(ISAMAFIA(playerid))
	{
		extract params -> new player:ID;else
			 SCM(playerid, COLOR_GRAD1, "Введите: /takephone [ID]");
		if(PlayerInfo[playerid][pRKInections] < 3)return SendClientMessage(playerid, COLOR_GREY,"Вы должны быть 3 рангом");
		if(takephone[ID] > false) return SendClientMessage(playerid, COLOR_GREY, "У него нет телефона");
		if (!ProxDetectorS(8.0, playerid, ID)) return SendClientMessage(playerid, COLOR_GREY, "Человек не рядом с вами!");
		if(ID == playerid) return SendClientMessage(playerid, COLOR_GREY, "Нельзя забрать телефон у самого себя!");
		format(stringer, sizeof(stringer), "%s забрал у Вас мобильный", PlayerInfo[playerid][pNames]);
		SendClientMessage(ID, 0x6ab1ffaa, stringer);
		takephone[ID] = true;
		format(stringer, sizeof(stringer), "%s забрал мобильный у %s", PlayerInfo[playerid][pNames] ,PlayerInfo[ID][pNames]);
	}
	else SendClientMessage(playerid, COLOR_GREY, "Вам недоступна данная команда!");
	return true;
}
cmd:showpass(playerid,params[])
{
	extract params -> new player:ID;else
		return SCM(playerid, COLOR_GRAD1, "Введите: /showpass [ID]");
	if(!IsPlayerConnected(ID)) return SendClientMessage(playerid, COLOR_RED, "[Ошибка] Человек не найден!");
	{
		if(ID != INVALID_PLAYER_ID)
		{
			if (!ProxDetectorS(8.0, playerid, ID)) return SendClientMessage(playerid, COLOR_RED, "Человек слишком далеко!");
			{
				new ttext[32];
				switch(PlayerInfo[playerid][pMRInections])
				{
				case 1: { ttext = "LSPD"; }
				case 2: { ttext = "FBI"; }
				case 3: { ttext = "Army SF"; }
				case 4: { ttext = "Мин.Здрав"; }
				case 5: { ttext = "Итальянская мафия"; }
				case 6: { ttext = "Японская мафия"; }
				case 7: { ttext = "Мэрия"; }
				case 8: { ttext = "Casino"; }

				case 10: { ttext = "RCPD"; }
				case 11: { ttext = "Инструкторы"; }
				case 12: { ttext = "Ballas Gang"; }
				case 13: { ttext = "Vagos Gang"; }
				case 14: { ttext = "Русская мафия"; }
				case 15: { ttext = "Grove Street Gang"; }
				case 16: { ttext = "San News"; }
				case 17: { ttext = "Aztecas Gang"; }
				case 18: { ttext = "Rifa Gang"; }
				case 19: { ttext = "Army LV"; }

				case 21: { ttext = "LVPD"; }

				case 23: { ttext = "Hitmans"; }
				case 24: { ttext = "Street Racers"; }
				case 25: { ttext = "S.W.A.T"; }
				case 26: { ttext = "Правительство"; }
				case 28: { ttext = "Армия LS"; }
				default: { ttext = "Гражданин"; }
				}
				new jtext[32];
				switch(PlayerInfo[playerid][pJob])
				{
				case 2: { jtext = "Механик"; }
				case 3: { jtext = "Продавец ХотДогов"; }
				case 4: { jtext = "Таксист"; }
				case 5: { jtext = "Развозчик продуктов"; }
				case 6: { jtext = "Тренер"; }
				case 7: { jtext = "Неизвестно"; }
				case 8: { jtext = "Инкасатор"; }
				case 9: { jtext = "Неизвестно"; }
				case 10: { jtext = "Мусоровоз"; }
				case 13: { jtext = "Автоугонщик"; }
				case 14: { jtext = "Боксёр"; }
				case 15: { jtext = "Местный DJ"; }
				default: { jtext = "Безработный"; }
				}
				new shltext[32];
				if(PlayerInfo[playerid][pGunLic]) { shltext = "Есть"; } else { shltext = "Нет"; }
				new level = PlayerInfo[playerid][pLevel];
				////////////////////////////////////////////////////////////
				SendClientMessage(playerid,COLOR_GREY, "-----------===[ PASSPORT ]===----------");
				SendMes(ID, COLOR_YELLOW2, "Имя: %s ",PlayerInfo[playerid][pNames]);
				SendMes(ID, COLOR_YELLOW2, "Возраст: %d  Телефон: %d",level,PlayerInfo[playerid][pPnumber]);
				if(PlayerInfo[playerid][pFacePass] == 1)//Настояший паспорт
				{
					SendMes(ID, COLOR_YELLOW2, "Фракция: %s",ttext);
					SendMes(ID, COLOR_YELLOW2, "Работа: %s",jtext);
					SendMes(ID, COLOR_YELLOW2, "Разрешение на оружие: %s",shltext);
					SendMes(ID, COLOR_YELLOW2, "Преступлений: %d",PlayerInfo[playerid][pKills]);
					SendMes(ID, COLOR_YELLOW2, "Законопослушность: %d",PlayerInfo[playerid][pZakonp]);
					if(PlayerInfo[playerid][pWanted] > 0) SendClientMessage(ID, COLOR_REDD, "Внимание! Это человек в розыске!");
				}
				if(PlayerInfo[playerid][pFacePass] == 2)//Гражданина
				{
					SendMes(ID, COLOR_YELLOW2, "Фракция: Гражданин");
					SendMes(ID, COLOR_YELLOW2, "Работа: Безработный");
					SendMes(ID, COLOR_YELLOW2, "Разрешение на оружие: Есть");
					SendMes(ID, COLOR_YELLOW2, "Преступлений: 0");
					SendMes(ID, COLOR_YELLOW2, "Законопослушность: 100");
				}
				if(PlayerInfo[playerid][pFacePass] == 3)//FBI
				{
					SendMes(ID, COLOR_YELLOW2, "Фракция: FBI",ttext);
					SendMes(ID, COLOR_YELLOW2, "Работа: Безработный");
					SendMes(ID, COLOR_YELLOW2, "Разрешение на оружие: Есть");
					SendMes(ID, COLOR_YELLOW2, "Преступлений: 0");
					SendMes(ID, COLOR_YELLOW2, "Законопослушность: 100");
				}
				if(PlayerInfo[playerid][pFacePass] == 4)//swat
				{
					SendMes(ID, COLOR_YELLOW2, "Фракция: S.W.A.T",ttext);
					SendMes(ID, COLOR_YELLOW2, "Работа: Безработный");
					SendMes(ID, COLOR_YELLOW2, "Разрешение на оружие: Есть");
					SendMes(ID, COLOR_YELLOW2, "Преступлений: 0");
					SendMes(ID, COLOR_YELLOW2, "Законопослушность: 100");
				}
				if(PlayerInfo[playerid][pFacePass] == 5)//правительство
				{
					SendMes(ID, COLOR_YELLOW2, "Фракция: Правительство",ttext);
					SendMes(ID, COLOR_YELLOW2, "Работа: Безработный");
					SendMes(ID, COLOR_YELLOW2, "Разрешение на оружие: Есть");
					SendMes(ID, COLOR_YELLOW2, "Преступлений: 0");
					SendMes(ID, COLOR_YELLOW2, "Законопослушность: 100");
				}
				////////////////////////////////////////////////////////////
				SendClientMessage(playerid,COLOR_GREY, "=============================");
				format(stringer,sizeof(stringer), "%s показал(а) свой паспорт %s'у", PlayerInfo[playerid][pNames], Name(ID));
				SetPlayerChatBubble(playerid,stringer,COLOR_PURPLE,30.0,10000);
				format(stringer, sizeof(stringer), "%s показал(a) свой паспорт", PlayerInfo[playerid][pNames]);
				ProxDetector(10.0, playerid, stringer, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			}
		}
	}
	return true;
}
cmd:showudost(playerid, params[])
{
	if(IsACop(playerid))
	{
		extract params -> new player:ID;else
			return SCM(playerid, COLOR_GRAD1, "Введите: /showudost [ID]");
		if(GetDistanceBetweenPlayers(ID, playerid) > 8.0)) return SendClientMessage(playerid, COLOR_GREY, "Вы слишком далеко!");
		{
			new ttext[32];
			if(PlayerInfo[playerid][pMRInections] == 0) { ttext = "Гражданин"; }
			else if(PlayerInfo[playerid][pMRInections] == 1) { ttext = "SAPD"; }
			else if(PlayerInfo[playerid][pMRInections] == 2) { ttext = "FBI"; }
			new rank = PlayerInfo[playerid][pRKInections];
			new level = PlayerInfo[playerid][pLevel];
			format(stringer, sizeof(stringer), "Имя: %s\nТелефон: %d\nВозвраст: %d\nПодразделение: %s\nРанг: %d", PlayerInfo[playerid][pNames],PlayerInfo[playerid][pPnumber],level,ttext,rank);
			ShowPlayerDialog(ID, 10036, DIALOG_STYLE_MSGBOX, "Удостоверение",stringer,"Закрыть","");
			format(stringer, sizeof(stringer), "%s показал своё удостоверение %s'у", PlayerInfo[playerid][pNames], PlayerInfo[ID][pNames]);
			ProxDetector(30.0, playerid, stringer, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
			format(stringer,sizeof(stringer), "показал своё удостоверение %s'у",PlayerInfo[ID][pNames]);
			SetPlayerChatBubble(playerid,stringer,COLOR_PURPLE,30.0,10000);
		}
	}
	return true;
}
cmd:openswat(playerid, params[])
{
	if(PlayerToPoint(5, playerid,1140.20984,-934.81818,44.51890))
	{
		if(PlayerInfo[playerid][pMRInections] == 25 || PlayerInfo[playerid][pLRInections] == 25)
		{
			if(swatopen)
			{
				MoveObject(swatgate, 1140.20984, -934.81818, 44.51890, 2.0);
				swatopen = false;
				SendClientMessage(playerid,COLOR_GREY,"Вы закрыли ворота.");
			}
			else
			{
				MoveObject (swatgate, 1140.20984,-934.81818,39.03890,2.0);
				swatopen = true;
				SendClientMessage(playerid,COLOR_GREY,"Вы открыли ворота. Для того, чтобы закрыть, введите команду повторно.");
			}
			return true;
		}
		else SendClientMessage(playerid,COLOR_GREY,"Вы не состоите в S.W.A.T");
	}
	return true;
}
cmd:gzcolor(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmInections] < 7) return true;
	extract params -> new l;else
		return SCM(playerid, COLOR_GRAD1, "Введите: /gzcolor [банда]");
	if(!IsInAllowedF(l)) return SendClientMessage(playerid,-1, " Введите: /gzcolor [банда]");
	for(new i = 0; i < 130; i++)
	{
		if(PlayerToKvadrat(playerid,GZInfo[i][gCoords][0], GZInfo[i][gCoords][1],GZInfo[i][gCoords][2],GZInfo[i][gCoords][3]))
		{
			GZInfo[i][gFrakVlad] = l;
			GangZoneStopFlashForAll(GZInfo[i][gID]);
			GangZoneHideForAll(GZInfo[i][gID]);
			GangZoneShowForAll(GZInfo[i][gID],GetGangZoneColor(i));
			getdate(year, month, day);
			gettime(hour,minuite,second);
			FixHour(hour);
			hour = shifthour;
			new data2[64];
			format(data2, 16, "%02d.%02d.%d в %02d:%d",day,month,year,hour,minuite);
			format(stringer, sizeof(stringer), "INSERT INTO `logs` (`Text`, `Date`) VALUES ('%s покрасил ганг зону %d','%s')",PlayerInfo[playerid][pNames],GZInfo[i][gID],data2);
			mysql_function_query(connects, stringer, false, "","");
			format(stringer, sizeof(stringer), "Администратор %s перекрасил ганг зону %d.",PlayerInfo[playerid][pNames], GZInfo[i][gID]);
			SendAdminMessage(COLOR_ORANGE, stringer);
			SaveGZ();
			return true;
		}
	}
	return true;
}
cmd:tazer(playerid, params[])
{
	if(!IsACop(playerid)) return true;
	{
		if(PlayerDeathmatch[playerid] > 0 && DeathmatchRound == 0) return SCM(playerid,COLOR_WHITE,"Запрещено использовать в бою!");
		if(IsPlayerInAnyVehicle(playerid)) return SCM(playerid, COLOR_GREY, "Невозможно использовать в машине!");
		if(Tazer[playerid] == 1) return  SCM(playerid, COLOR_GREY, "Перезарядка тайзера ещё не прошла!");
		new suspect = GetClosestPlayer(playerid);
		if(PlayerCuffed[suspect] > 0) return  SCM(playerid, COLOR_GREY, "Игрок уже в наручниках!");
		if(GetDistanceBetweenPlayers(playerid,suspect) > 5) return SCM(playerid, COLOR_GREY, "Рядом с тобой никого нет!");
		if(IsACop(suspect)) return SCM(playerid, COLOR_GREY, "Вы не можете ударить тазером законника");
		if(IsPlayerInAnyVehicle(suspect)) return SCM(playerid, COLOR_GREY, "Человек в машине!");
		format(stringer, sizeof(stringer), "Вас обездвижил электрошокером %s на 15 секунд.", PlayerInfo[playerid][pNames]);
		SCM(suspect, 0x6495EDFF, stringer);
		format(stringer, sizeof(stringer), "Вы обездвижили электрошокером %s на 15 секунд.", PlayerInfo[suspect][pNames]);
		SCM(playerid, 0x6495EDFF, stringer);
		format(stringer, sizeof(stringer), "%s проводит задержание %s ",PlayerInfo[playerid][pNames] ,PlayerInfo[suspect][pNames]);
		ProxDetector(10.0, playerid, stringer, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
		TogglePlayerControllable(suspect, 0);
		SetPlayerSpecialAction(suspect,SPECIAL_ACTION_HANDSUP);
		PlayerCuffed[suspect] = 1;
		PlayerCuffedTime[suspect] = 15;
		Tazer[playerid] = 1;
		TazerTime[playerid] = 10;
	}
	return true;
}
cmd:cuff(playerid, params[])
{
	if(!IsACop(playerid)) return true;
	extract params -> new player:ID;else
		return SCM(playerid, COLOR_GRAD1, "Введите: /cuff [ID]");
	if(PlayerTied[playerid] > 0)  return SCM(playerid, COLOR_GREY, "Вы связаны!");
	if(ID == playerid) return SCM(playerid, COLOR_GREY, "Вы не можете надеть наручники на самого себя!");
	if(IsACop(ID)) return SCM(playerid, COLOR_GREY, "Вы не можете надеть наручники на законника");
	if(PlayerCuffed[ID] == 2) return SCM(playerid, COLOR_GREY, "На этого человека уже одеты наручники");
	if(PlayerInfo[ID][pWanted] == 0) return SendClientMessage(playerid, COLOR_GREY, "Вы не можете надеть наручники на этого человека (он не в розыске)");
	if(GetDistanceBetweenPlayers(ID, playerid) > 2.0) return SCM(playerid, COLOR_GREY, "Этот игрок не рядом с Вами");
	if(PlayerCuffed[ID] != 1) return SendClientMessage(playerid, COLOR_GREY, "Игрок не оглушен, используйте /tazer.");
	if(IsACop(playerid))
	{
		format(stringer, sizeof(stringer), "На Вас надел наручники %s.", PlayerInfo[playerid][pNames]);
		SendClientMessage(ID, 0x6ab1ffaa, stringer);
		format(stringer, sizeof(stringer), "Вы надели наручники на %s.", PlayerInfo[ID][pNames]);
		SendClientMessage(playerid, 0x6ab1ffaa, stringer);
		format(stringer, sizeof(stringer), "%s надел(а) на %s наручники.", PlayerInfo[playerid][pNames] ,PlayerInfo[ID][pNames]);
		SetPlayerAttachedObject(ID, 0, 19418, 6, -0.011000, 0.028000, -0.022000, -15.600012, -33.699977, -81.700035, 0.891999, 1.000000, 1.168000);
		GameTextForPlayer(ID, "~r~Cuffed", 2500, 3);
		ProxDetector(30.0, playerid, stringer, COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE,COLOR_PURPLE);
		SetPlayerSpecialAction(ID,SPECIAL_ACTION_CUFFED);
		TogglePlayerControllable(ID, false);
		PlayerCuffed[ID] = 2;
		PlayerCuffedTime[ID] = 3600;
	}
	return true;	
}
cmd:uncuff(playerid, params[])
{
	if(!IsACop(playerid)) return true;
	extract params -> new player:ID;else
		return SCM(playerid, COLOR_GRAD1, "Введите: /uncuff [ID]");
	if(PlayerTied[playerid] > 0) return SCM(playerid, COLOR_GREY, "Вы связаны!");
	if(ID == playerid) { SendClientMessage(playerid, COLOR_GREY, "Вы не можете снять наручники с самого себя!"); return true; }
	if(GetDistanceBetweenPlayers(ID, playerid) > 5.0) return SCM(playerid, COLOR_GREY, "Этот игрок не рядом с Вами");
	if(PlayerCuffed[ID] == 2)
	{
		format(stringer, sizeof(stringer), "С Вас снял наручники %s.", PlayerInfo[playerid][pNames]);
		SendClientMessage(ID, 0x6ab1ffaa, stringer);
		GameTextForPlayer(ID, "~g~Uncuffed", 2500, 3);
		format(stringer, sizeof(stringer), "Вы сняли наручники с %s.", PlayerInfo[ID][pNames]);
		SendClientMessage(playerid, 0x6ab1ffaa, stringer);
		SetPlayerSpecialAction(ID,SPECIAL_ACTION_NONE); RemovePlayerAttachedObject(ID,0); TogglePlayerControllable(ID, true); PlayerCuffed[ID] = 0;
	}
	else return SCM(playerid, COLOR_GREY, "Этот игрок не в наручниках!"); 
	return true;		
}
cmd:find(playerid, params[])
{
	if(IsACop(playerid) || PlayerInfo[playerid][pMRInections] == 23)
	{
		extract params -> new player:ID;else
			return SCM(playerid, COLOR_GRAD1, "Введите: /find [ID]");
		if(ID == playerid) { SCM(playerid, COLOR_GREY, "[Ошибка] Вы указали свой ID"); return true; }
		if(PlayerInfo[ID][pInt] > 0) SCM(playerid,COLOR_GREY,"Человек находиться в интерьере.");
		new Float:X,Float:Y,Float:Z;
		GetPlayerPos(ID, X,Y,Z);
		SetPlayerCheckpoint(playerid, X,Y,Z, 3);
	}
	return true;	
}
cmd:refill(playerid, params[])
{
	if(PlayerInfo[playerid][pJob] != 2) return SCM(playerid, COLOR_GRAD1, "Вы не работаете механиком!");
	if(gcontract[playerid] != 1) return SCM(playerid, COLOR_GRAD1, "У Вас не подписан контракт с заправкой!");
	extract params -> new player:ID, value;else
		return SCM(playerid, COLOR_GRAD1, " Введите: /refill [id] [цена]");
	if(value < 1 || value > 5000) return SCM(playerid, COLOR_GRAD1, "Нельзя меньше 1, и больше 5000!");
	if(GetVehicleModel(GetPlayerVehicleID(playerid)) != 525) return SCM(playerid, COLOR_GREY, "Вы не на эвакуаторе!");
	if(GetDistanceBetweenPlayers(ID, playerid) < 8.0) && IsPlayerInAnyVehicle(ID))
	{
		if(ID == playerid) return SCM(playerid, COLOR_GREY, "[Ошибка] Вы указали свой ID!");
		format(stringer, sizeof(stringer), "Вы предложили %s заправить его машину за %d вирт.",Name(ID), value);
		SendClientMessage(playerid, 0x6ab1ffaa, stringer);
		format(stringer, sizeof(stringer), "Механик %s предлагает заправить вашу машину за %d вирт.(Введите /accept refill), чтобы потвердить заправку.",Name(playerid),value);
		SendClientMessage(ID, 0x6ab1ffaa, stringer);
		RefillOffer[ID] = playerid;
		RefillPrice[ID] = value;
	}
	else { SCM(playerid, COLOR_GREY, "Он не в машине!"); }
	return true;
}	
cmd:eject(playerid,params[])
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new State=GetPlayerState(playerid);
		if(State!=PLAYER_STATE_DRIVER) return SendClientMessage(playerid,COLOR_GREY,"Вы не водитель!");
		extract params -> new player:ID;else
			return SCM(playerid, COLOR_GRAD1, " Введите: /refill [id] [цена]");
		new test = GetPlayerVehicleID(playerid);
		if(ID == playerid) { SendClientMessage(playerid, COLOR_GREY, "Вы не можете выкинуть из машины себя!"); return true; }
		if(IsPlayerInVehicle(ID,test))
		{
			if(PlayerCuffed[ID] > 0)
			{
				RemoveFromVehicle(ID);
				format(stringer, sizeof(stringer), "Вы выкинули из машины %s, наручники сломались!", PlayerInfo[ID][pNames]);
				SendClientMessage(playerid, 0x6495EDFF, stringer);
				format(stringer, sizeof(stringer), "Вас выкинул из машины %s, наручники сломались!!", PlayerInfo[playerid][pNames]);
				SendClientMessage(ID, 0x6495EDFF, stringer);
				TogglePlayerControllable(ID, 1);
				SetPlayerSpecialAction(ID,SPECIAL_ACTION_NONE);
				RemovePlayerAttachedObject(ID,0);
				PlayerCuffed[ID] = 0;
			}
			else
			{
				format(stringer, sizeof(stringer), "Вы выкинули из машины %s!", PlayerInfo[ID][pNames]);
				SendClientMessage(playerid, 0x6495EDFF, stringer);
				format(stringer, sizeof(stringer), "Вас выкинул из машины %s!", PlayerInfo[playerid][pNames]);
				SendClientMessage(ID, 0x6495EDFF, stringer);
				RemoveFromVehicle(ID);
				PlayerTied[ID] = 0;
				TogglePlayerControllable(ID, 1);
			}
		}
		else return SendClientMessage(playerid, COLOR_GREY, "Не рядом с вами!");
	}
	else return SendClientMessage(playerid, COLOR_GREY, "Вы должны быть в машине!");
	return true;
}