import java.io.*;
import java.util.*;
import java.text.*;

/**
 * encapsulates a Close Combat 3 saved game file
 */

public class SaveGame { 
	byte buffer[] = new byte[226404];
	String pathName;
	
	Vector germanTeams;
	Vector russianTeams;
	Vector soldiers;
	Vector weapons;
	
	SaveGame(String pathName) {
		this.pathName = pathName;
		
		// create new File object for accessing the zfx file
		try {
			RandomAccessFile f = new RandomAccessFile(pathName, "r");
			f.read(buffer, 0, buffer.length);
			f.close();
		} catch (IOException e) {
		}
		
		// read GEteams.txt
		try {
			
			RandomAccessFile f = new RandomAccessFile("c:\\games\\cc3\\data\\base\\GEteams.txt", "r");
			byte[] b = new byte[(int)f.length()];
			f.read(b, 0, b.length);
			f.close();
			
			StringTokenizer st = new StringTokenizer(new String(b), "\r\n");
			germanTeams = new Vector();
			
			// skip the first two lines (they are just header info)
			st.nextToken();
			st.nextToken();
			while (st.hasMoreTokens()) {
				String line = st.nextToken();
				germanTeams.addElement(line);
			}
			
		} catch (IOException e) {
		}
		
		// read RUteams.txt
		try {
			
			RandomAccessFile f = new RandomAccessFile("c:\\games\\cc3\\data\\base\\RUteams.txt", "r");
			byte[] b = new byte[(int)f.length()];
			f.read(b, 0, b.length);
			f.close();
			
			StringTokenizer st = new StringTokenizer(new String(b), "\r\n");
			russianTeams = new Vector();
			
			// skip the first two lines (they are just header info)
			st.nextToken();
			st.nextToken();
			while (st.hasMoreTokens()) {
				String line = st.nextToken();
				russianTeams.addElement(line);
			}
			
		} catch (IOException e) {
		}

		// read soldiers.txt
		try {
			
			RandomAccessFile f = new RandomAccessFile("c:\\games\\cc3\\data\\base\\soldiers.txt", "r");
			byte[] b = new byte[(int)f.length()];
			f.read(b, 0, b.length);
			f.close();
			
			StringTokenizer st = new StringTokenizer(new String(b), "\r\n");
			soldiers = new Vector();
			
			// skip the first two lines (they are just header info)
			st.nextToken();
			st.nextToken();
			while (st.hasMoreTokens()) {
				String line = st.nextToken();
				soldiers.addElement(line);
			}
			
		} catch (IOException e) {
		}

		// read weapons.txt
		try {
			
			RandomAccessFile f = new RandomAccessFile("c:\\games\\cc3\\data\\base\\weapons.txt", "r");
			byte[] b = new byte[(int)f.length()];
			f.read(b, 0, b.length);
			f.close();
			
			StringTokenizer st = new StringTokenizer(new String(b), "\r\n");
			weapons = new Vector();
			
			// skip the first two lines (they are just header info)
			st.nextToken();
			st.nextToken();
			while (st.hasMoreTokens()) {
				String line = st.nextToken();
				weapons.addElement(line);
			}
			
		} catch (IOException e) {
		}
	}
	
	boolean side = GERMANS; // false=russian,true=german
	static final boolean RUSSIANS = false;
	static final boolean GERMANS  = true;
	void setSide(boolean side) {
		this.side = side;
	}
	
	boolean getSide() {
		return side;
	}
	
	String getString(int offset, int maxLength) {
		return new String(buffer, offset, maxLength);
	}
	
	int getInt(int offset) {
		int b0 = ((int)buffer[offset]) & 0xff;
		int b1 = ((int)buffer[offset+1]) & 0xff;
		int b2 = ((int)buffer[offset+2]) & 0xff;
		int b3 = ((int)buffer[offset+3]) & 0xff;
		
		int value = b0 | b1<<8 | b2<<16 | b3<<24;

		return value;
	}

	short getShort(int offset) {
		// trick to widen bytes without doing sign extension
		int b0 = ((int)buffer[offset]) & 0xff;
		int b1 = ((int)buffer[offset+1]) & 0xff;
		short value = (short)(b0 | b1<<8);
		return value;
	}
	
	byte getByte(int offset) {
		return buffer[offset];
	}
	
	
    static final int OPERATION_NAME = 0;    static final int OPERATION_NAME_LENGTH = 260;
    
	String getOperationName() {
		return getString(OPERATION_NAME,OPERATION_NAME_LENGTH);
	}

    
    static final int GERMAN_SOLDIER  = 198892;  // start of german soldier records
    static final int RUSSIAN_SOLDIER = 169904;  // start of russian soldier records

    static final int SOLDIER_COUNT  = 160;      // number of soldiers per side in the file
    static final int SOLDIER_LENGTH = 148;      // number of bytes for each soldier record
    
    static final int SOLDIER_NAME           = 0;    static final int SOLDIER_NAME_LENGTH = 26;
    static final int SOLDIER_TYPE           = 26;   // short
    static final int SOLDIER_RANK           = 28;   // byte
    static final int SOLDIER_LEADERSHIP     = 32;   // int
    static final int SOLDIER_STRENGTH       = 36;   // int
    static final int SOLDIER_INTELLIGENCE   = 40;   // int
    static final int SOLDIER_EXPERIENCE     = 44;   // int
    static final int SOLDIER_MORALE         = 48;   // int
    // unknown fields (8 bytes)
    static final int SOLDIER_MEDAL0         = 60;   // int
    static final int SOLDIER_MEDAL1         = 64;   // int
    static final int SOLDIER_MEDAL2         = 68;   // int
    static final int SOLDIER_MEDAL3         = 72;   // int
    static final int SOLDIER_MEDAL4         = 76;   // int
    static final int SOLDIER_MEDAL5         = 80;   // int
    static final int SOLDIER_HEALTH         = 84;   // int (1=normal, 2=injured, 3=incapacitated, 4=dead) 
    static final int SOLDIER_TANKS          = 88;   // short
    static final int SOLDIER_VEHICLES       = 90;   // short
    static final int SOLDIER_GUNS           = 92;   // short
    static final int SOLDIER_SOLDIERS       = 94;   // short
    // unknown fields (2 bytes)
    static final int SOLDIER_BRAVERY        = 98;   // short
    static final int SOLDIER_COWARDICE      = 100;  // short
    // unknown fields (42 bytes) (history & skills???)
    static final int SOLDIER_NUMBER         = 144;  // short
    
    
    static final int GERMAN_TEAM  = 223924;
    static final int RUSSIAN_TEAM = 194936;

    static final int TEAM_COUNT      = 16;  // number of teams per side in the file
    static final int TEAM_LENGTH     = 120; // number of bytes for each team record

    static final int TEAM_NAME       = 0;   static final int TEAM_NAME_LENGTH = 26; 
    static final int TEAM_TYPE       = 26;  // short (-1=no team, look up in teams.txt)
    static final int TEAM_SOLDIER0   = 28;  // short (soldier index)
    static final int TEAM_SOLDIER1   = 30;  // short
    static final int TEAM_SOLDIER2   = 32;  // short
    static final int TEAM_SOLDIER3   = 34;  // short
    static final int TEAM_SOLDIER4   = 36;  // short
    static final int TEAM_SOLDIER5   = 38;  // short
    static final int TEAM_SOLDIER6   = 40;  // short
    static final int TEAM_SOLDIER7   = 42;  // short
    static final int TEAM_SOLDIER8   = 44;  // short
    static final int TEAM_SOLDIER9   = 46;  // short
    static final int TEAM_VEHICLE    = 48;  // ?? 
    static final int TEAM_MORALE     = 52;  // int
    static final int TEAM_EXPERIENCE = 56;  // int
    static final int TEAM_VALUE      = 60;  // short
    static final int TEAM_FATIGUE    = 64;  // int
    static final int TEAM_RESTING    = 68;  // byte
    static final int TEAM_NUMBER     = 116; // short
    static final int TEAM_SIDE       = 118; // byte (0=russian, 1=german)
	
	int getSoldierOffset(int soldierIndex) {
		int offset = 0;
		if (getSide() == GERMANS) {
			offset = GERMAN_SOLDIER;
		} else {
			offset = RUSSIAN_SOLDIER;
		}
		offset += soldierIndex*SOLDIER_LENGTH;
		return offset;
	}

	String getSoldierName(int soldierIndex) {
		return getString(getSoldierOffset(soldierIndex) + SOLDIER_NAME, SOLDIER_NAME_LENGTH);
	}
	
	int getSoldierByte(int soldierIndex, int offset) {
		return getByte(getSoldierOffset(soldierIndex) + offset);
	}
	int getSoldierShort(int soldierIndex, int offset) {
		return getShort(getSoldierOffset(soldierIndex) + offset);
	}
	int getSoldierInt(int soldierIndex, int offset) {
		return getInt(getSoldierOffset(soldierIndex) + offset);
	}
	
	
/*
null
Private
Corporal
Sergeant
Master Sgt
1st Sergeant
2nd Lieutenant
1st Lieutenant
Captian
Major
*/
	
	int getSoldierRankIndex(int soldierIndex) {
		int rankIndex = getByte(getSoldierOffset(soldierIndex) + SOLDIER_RANK);
		return rankIndex;
	}

	static final String[] ranks = {"","Pvt ","Cpl ","Sgt ","MSgt ","1st Sgt ","2nd Lt ","1st Lt ","Capt ","Maj " };

	String getSoldierRank(int soldierIndex) {
		int rankIndex = getSoldierRankIndex(soldierIndex);
		if (rankIndex > ranks.length)
			return Integer.toString(rankIndex) + " ";
		else
			return new String(ranks[rankIndex]);
	}
	
	int getSoldierLeadership(int soldierIndex) {
		return getInt(getSoldierOffset(soldierIndex) + SOLDIER_LEADERSHIP);
	}

	int getSoldierStrength(int soldierIndex) {
		return getInt(getSoldierOffset(soldierIndex) + SOLDIER_STRENGTH);
	}
	
	int getSoldierIntelligence(int soldierIndex) {
		return getInt(getSoldierOffset(soldierIndex) + SOLDIER_INTELLIGENCE);
	}
	
	int getSoldierExperience(int soldierIndex) {
		return getInt(getSoldierOffset(soldierIndex) + SOLDIER_EXPERIENCE);
	}
	
	int getSoldierMorale(int soldierIndex) {
		return getInt(getSoldierOffset(soldierIndex) + SOLDIER_MORALE);
	}
	
	int getSoldierHealth(int soldierIndex) {
		return getInt(getSoldierOffset(soldierIndex) + SOLDIER_HEALTH);
	}
	
	int getSoldierBravery(int soldierIndex) {
		return getShort(getSoldierOffset(soldierIndex) + SOLDIER_BRAVERY);
	}
	int getSoldierCowerdice(int soldierIndex) {
		return getShort(getSoldierOffset(soldierIndex) + SOLDIER_COWARDICE);
	}
	int getSoldierSoldiers(int soldierIndex) {
		return getShort(getSoldierOffset(soldierIndex) + SOLDIER_SOLDIERS);
	}
	int getSoldierTanks(int soldierIndex) {
		return getShort(getSoldierOffset(soldierIndex) + SOLDIER_TANKS);
	}
	int getSoldierVehicles(int soldierIndex) {
		return getShort(getSoldierOffset(soldierIndex) + SOLDIER_VEHICLES);
	}
	int getSoldierGuns(int soldierIndex) {
		return getShort(getSoldierOffset(soldierIndex) + SOLDIER_GUNS);
	}
	
	int getSoldierMedal(int soldierIndex, int medalIndex) {
		return getInt(getSoldierOffset(soldierIndex) + SOLDIER_MEDAL0 + medalIndex*4);
	}

	
	int getTeamOffset(int teamIndex) {
		int offset = 0;
		if (getSide() == GERMANS) {
			offset = GERMAN_TEAM;
		} else {
			offset = RUSSIAN_TEAM;
		}
		offset += teamIndex*TEAM_LENGTH;
		return offset;
	}

	String getTeamName(int teamIndex) {
		return getString(getTeamOffset(teamIndex) + TEAM_NAME, TEAM_NAME_LENGTH);
	}
	
	int getTeamMorale(int teamIndex) {
		return getInt(getTeamOffset(teamIndex) + TEAM_MORALE);
	}
	
	int getTeamExperience(int teamIndex) {
		return getInt(getTeamOffset(teamIndex) + TEAM_EXPERIENCE);
	}
	
	static final int MASTERTEAMS_NAME				=  0;	// string
	static final int MASTERTEAMS_TYPE_NAME			=  1;	// string
	static final int MASTERTEAMS_FULL_NAME			=  2;	// string
	static final int MASTERTEAMS_NATION				=  3;	// string
	static final int MASTERTEAMS_CLASS				=  4;
	static final int MASTERTEAMS_TYPE				=  5;
	static final int MASTERTEAMS_VALUE				=  6;
	static final int MASTERTEAMS_EXPERIENCE			=  7;
	static final int MASTERTEAMS_MORALE				=  8;
	static final int MASTERTEAMS_STYLE				=  9;
	static final int MASTERTEAMS_ICON				= 10;
	static final int MASTERTEAMS_NAME_GRAPHIC		= 11;
	static final int MASTERTEAMS_WINTER_EQUIPPED	= 12;
	static final int MASTERTEAMS_REQUISITION_TYPE	= 13;
	static final int MASTERTEAMS_SAI_UNIT			= 14;
	static final int MASTERTEAMS_SOLDIER_CLASS_0	= 15;	// -1 if team does not use this slot
	static final int MASTERTEAMS_SOLDIER_CLASS_1	= 16;
	static final int MASTERTEAMS_SOLDIER_CLASS_2	= 17;
	static final int MASTERTEAMS_SOLDIER_CLASS_3	= 18;
	static final int MASTERTEAMS_SOLDIER_CLASS_4	= 19;
	static final int MASTERTEAMS_SOLDIER_CLASS_5	= 20;
	static final int MASTERTEAMS_SOLDIER_CLASS_6	= 21;
	static final int MASTERTEAMS_SOLDIER_CLASS_7	= 22;
	static final int MASTERTEAMS_SOLDIER_CLASS_8	= 23;
	static final int MASTERTEAMS_SOLDIER_CLASS_9	= 24;
	static final int MASTERTEAMS_SOLDIER_TYPE_0		= 25;	// -1 if team does not use this slot
	static final int MASTERTEAMS_SOLDIER_TYPE_1		= 26;
	static final int MASTERTEAMS_SOLDIER_TYPE_2		= 27;
	static final int MASTERTEAMS_SOLDIER_TYPE_3		= 28;
	static final int MASTERTEAMS_SOLDIER_TYPE_4		= 29;
	static final int MASTERTEAMS_SOLDIER_TYPE_5		= 30;
	static final int MASTERTEAMS_SOLDIER_TYPE_6		= 31;
	static final int MASTERTEAMS_SOLDIER_TYPE_7		= 32;
	static final int MASTERTEAMS_SOLDIER_TYPE_8		= 33;
	static final int MASTERTEAMS_SOLDIER_TYPE_9		= 34;
	static final int MASTERTEAMS_VEHICLE_CLASS		= 35;
	static final int MASTERTEAMS_UPGRADE_PATH_1		= 36;
	static final int MASTERTEAMS_UPGRADE_PATH_2		= 37;
	static final int MASTERTEAMS_UPGRADE_PATH_3		= 38;
	static final int MASTERTEAMS_RARITY_SUMMER_41	= 39;
	static final int MASTERTEAMS_RARITY_FALL_41		= 40;
	static final int MASTERTEAMS_RARITY_WINTER_42	= 41;
	static final int MASTERTEAMS_RARITY_SPRING_42	= 42;
	static final int MASTERTEAMS_RARITY_SUMMER_42	= 43;
	static final int MASTERTEAMS_RARITY_FALL_42     = 44;
	static final int MASTERTEAMS_RARITY_WINTER_43   = 45;
	static final int MASTERTEAMS_RARITY_SPRING_43   = 46;
	static final int MASTERTEAMS_RARITY_SUMMER_43   = 47;
	static final int MASTERTEAMS_RARITY_FALL_43     = 48;
	static final int MASTERTEAMS_RARITY_WINTER_44   = 49;
	static final int MASTERTEAMS_RARITY_SPRING_44   = 50;
	static final int MASTERTEAMS_RARITY_SUMMER_44   = 51;
	static final int MASTERTEAMS_RARITY_FALL_44     = 52;
	static final int MASTERTEAMS_RARITY_WINTER_45   = 53;
	static final int MASTERTEAMS_RARITY_SPRING_45   = 54;
	static final int MASTERTEAMS_RARITY_MINEDOG     = 55;
	static final int MASTERTEAMS_RARITY_DESCRIPTION = 56;	// string
	
	String getMasterTeamString(int teamIndex, int column) {
		short teamType = getTeamType(teamIndex);
		if (teamType < 0)
			return "teamType (" + Short.toString(teamType) + ") <= 0";
		if (getSide() == GERMANS && teamType >= germanTeams.size())
			return "teamType (" + Short.toString(teamType) + ") >= germanTeams.size()";
		if (getSide() == RUSSIANS && teamType >= russianTeams.size())
			return "teamType (" + Short.toString(teamType) + ") >= russianTeams.size()";
		
		String line;
		if (getSide() == GERMANS) {
			line = (String)germanTeams.elementAt(teamType);
		} else {
			line = (String)russianTeams.elementAt(teamType);
		}
		
		StringTokenizer st = new StringTokenizer(line, "\t");
		for (int i = 0; i < column; i++) {
			st.nextToken();
		}
		return st.nextToken();
	}
	
	int getMasterTeamInt(int teamIndex, int column) {
		String s = getMasterTeamString(teamIndex, column);
		int value = 0;
		try {
			value = Integer.parseInt(s);
		} catch (Exception e) {
		}
		
		return value;
	}
	
	String getTeamDescription(int teamIndex) {
		return getMasterTeamString(teamIndex, MASTERTEAMS_FULL_NAME);
	}
	
	short getTeamType(int teamIndex) {
		return getShort(getTeamOffset(teamIndex) + TEAM_TYPE);
	}
	
	int getTeamMemberCount(int teamIndex) {
		int teamMemberCount = 0;
		while (teamMemberCount < 10 && getMasterTeamInt(teamIndex, MASTERTEAMS_SOLDIER_CLASS_0+teamMemberCount) >= 0)
			teamMemberCount++;
		return teamMemberCount;
	}
	
	final static String[] TEAM_MEMBER_TYPE_DESCRIPTIONS = {
		"Soldier",
		"Leader",
		"Crew",
		"Asst Leader",
		"Commander",
		"Tank Leader"
	};
	
	String getTeamMemberTypeDescription(int teamIndex, int teamMemberIndex) {
		return TEAM_MEMBER_TYPE_DESCRIPTIONS[getTeamMemberType(teamIndex,teamMemberIndex)];
	}
	
	int getTeamMemberType(int teamIndex, int teamMemberIndex) {
		return getMasterTeamInt(teamIndex, MASTERTEAMS_SOLDIER_TYPE_0+teamMemberIndex);
	}

	int getTeamMemberClass(int teamIndex, int teamMemberIndex) {
		return getMasterTeamInt(teamIndex, MASTERTEAMS_SOLDIER_CLASS_0+teamMemberIndex);
	}

	int getTeamSoldierIndex(int teamIndex, int soldierSlot) {
		return getShort(getTeamOffset(teamIndex) + TEAM_SOLDIER0 + soldierSlot*2);
	}
	

	//////////////////////////////////////////////////////////////////////////////
	// soldier.txt parsing functions
	
	static final int MASTERSOLDIERS_WEAPONNAME			=  0;	// string
	static final int MASTERSOLDIERS_NATION				=  1;	// string
	static final int MASTERSOLDIERS_CLASS				=  2; 
	static final int MASTERSOLDIERS_ARMOR_HEAD			=  3;
	static final int MASTERSOLDIERS_ARMOR_BODY			=  4;
	static final int MASTERSOLDIERS_TO_HIT_HEAD			=  5;	// chance out of 128 (64 = 50%)
	static final int MASTERSOLDIERS_TO_HIT_BODY			=  6;
	static final int MASTERSOLDIERS_WEIGHT				=  7;	// does not include ammo or weapon
	static final int MASTERSOLDIERS_CREWED_WEAPON		=  8;
	static final int MASTERSOLDIERS_CREWED_AP			=  9;
	static final int MASTERSOLDIERS_CREWED_HE			= 10;
	static final int MASTERSOLDIERS_CREWED_HEAT			= 11;
	static final int MASTERSOLDIERS_CREWED_SMOKE		= 12;
	static final int MASTERSOLDIERS_CREWED_SPEC			= 13;
	static final int MASTERSOLDIERS_PRIMARY_WEAPON		= 14;
	static final int MASTERSOLDIERS_PRIMARY_AP			= 15;
	static final int MASTERSOLDIERS_PRIMARY_HE			= 16;
	static final int MASTERSOLDIERS_PRIMARY_HEAT		= 17;
	static final int MASTERSOLDIERS_PRIMARY_SMOKE		= 18;
	static final int MASTERSOLDIERS_PRIMARY_SPEC		= 19;
	static final int MASTERSOLDIERS_SECONDARY_WEAPON	= 20;
	static final int MASTERSOLDIERS_SECONDARY_AP		= 21;
	static final int MASTERSOLDIERS_SECONDARY_HE		= 22;
	static final int MASTERSOLDIERS_SECONDARY_HEAT		= 23;
	static final int MASTERSOLDIERS_SECONDARY_SMOKE		= 24;
	static final int MASTERSOLDIERS_SECONDARY_SPEC		= 25;
	static final int MASTERSOLDIERS_GRENADE_WEAPON		= 26;
	static final int MASTERSOLDIERS_GRENADE_AP			= 27;
	static final int MASTERSOLDIERS_GRENADE_HE			= 28;
	static final int MASTERSOLDIERS_GRENADE_HEAT		= 29;
	static final int MASTERSOLDIERS_GRENADE_SMOKE		= 30;
	static final int MASTERSOLDIERS_GRENADE_SPEC		= 31;
	
	String getMasterSoldierString(int row, int column) {
		
		// get entire line for the specified row
		String line = (String)soldiers.elementAt(row);
		
		// get value in the specified column
		StringTokenizer st = new StringTokenizer(line, "\t");
		for (int i = 0; i < column; i++) {
			st.nextToken();
		}
		return st.nextToken();
	}
	
	int getMasterSoldierInt(int row, int column) {
		// get string version of element
		String s = getMasterSoldierString(row, column);
		
		// translate it to an int
		int value = 0;
		try {
			value = Integer.parseInt(s);
		} catch (Exception e) {
		}
		return value;
	}
	

	String getMasterSoldierWeaponName(int soldierClass) {
		return getMasterSoldierString(soldierClass, MASTERSOLDIERS_WEAPONNAME);
	}
	
	//////////////////////////////////////////////////////////////////////////////
	// weapons.txt parsing functions
	
	static final int MASTERWEAPONS_NAME					=  0;	// string
	static final int MASTERWEAPONS_NATION				=  1;	// string
	
	String getMasterWeaponString(int row, int column) {
		
		// get entire line for the specified row
		String line = (String)weapons.elementAt(row);
		
		// get value in the specified column
		StringTokenizer st = new StringTokenizer(line, "\t");
		for (int i = 0; i < column; i++) {
			st.nextToken();
		}
		return st.nextToken();
	}
	
	int getMasterWeaponInt(int row, int column) {
		// get string version of element
		String s = getMasterSoldierString(row, column);
		
		// translate it to an int
		int value = 0;
		try {
			value = Integer.parseInt(s);
		} catch (Exception e) {
		}
		return value;
	}
	
}
