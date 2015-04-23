void RFEnqueueCharacter(RFClientConnection *client, uint32_t len, char *data){
	static MYSQL_STMT *insertStmt=NULL;
	static MYSQL_BIND insertBinds[3];
	static char toonZone[5]={0}, toonRealm[20]={0}, toonName[60]={0};	
	static unsigned long zoneLen=0, realmLen=0, nameLen=0;
	char *tokZone=NULL, *tokRealm=NULL, *tokName=NULL;
	const char insertQuery[]=	
		"INSERT INTO character_info ( "
		"	Zone, "
		"	Realm, "
		"	Toon, "
		"	InQueue, "
		"	QueuedAt, "
		"	InProgress "
		") VALUES ( "
		"	?,  "
		"	?, "  
		"	?, "
		"	1,  "
		"	UTC_TIMESTAMP, "
		"	NULL "
		") ON DUPLICATE KEY UPDATE InQueue=1, QueuedAt=UTC_TIMESTAMP, InProgress=NULL ";

	if (!insertStmt){
		memset(insertBinds, 0, sizeof(insertBinds));

		insertBinds[0].buffer_type = MYSQL_TYPE_STRING;
		insertBinds[0].buffer = toonZone;
		insertBinds[0].buffer_length = sizeof(toonZone);
		insertBinds[0].length = &zoneLen;

		insertBinds[1].buffer_type = MYSQL_TYPE_STRING;
		insertBinds[1].buffer = toonRealm;
		insertBinds[0].buffer_length = sizeof(toonRealm);
		insertBinds[1].length = &realmLen;

		insertBinds[2].buffer_type = MYSQL_TYPE_STRING;
		insertBinds[2].buffer = toonName;
		insertBinds[0].buffer_length = sizeof(toonName);
		insertBinds[2].length = &nameLen;

		insertStmt = rf_mysql_stmt_init();
		if (!insertStmt){
			rf_mysql_print_errors();
			insertStmt=NULL;
			return;
		}

		if (mysql_stmt_prepare(insertStmt, insertQuery, sizeof(insertQuery)) > 0){
			rf_mysql_print_errors();
			mysql_stmt_close(insertStmt);
			insertStmt=NULL;
			return;
		}

		if (mysql_stmt_bind_param(insertStmt, insertBinds) > 0){
			rf_mysql_print_errors();
			mysql_stmt_close(insertStmt);
			insertStmt=NULL;
			return;
		}
	}
	
	tokZone = mystrtok(data);
	while (tokZone){
		tokRealm = mystrtok(NULL);
		tokName = mystrtok(NULL);

		if (tokName && tokRealm){
			memset(toonZone, 0, sizeof(toonZone));
			memset(toonRealm, 0, sizeof(toonRealm));
			memset(toonName, 0, sizeof(toonName));

			
			strncpy(toonZone, tokZone, sizeof(toonZone));
			strncpy(toonRealm, tokRealm, sizeof(toonRealm));
			strncpy(toonName, tokName, sizeof(toonName));

			zoneLen=strlen(toonZone);
			realmLen=strlen(toonRealm);
			nameLen=strlen(toonName);

			rf_ensure_connection();
			mysql_stmt_execute(insertStmt);
		}

		tokZone = mystrtok(NULL);
	}
	RFSendPacket(client, RF_PACKET_SUCCESS, 0, NULL);
}