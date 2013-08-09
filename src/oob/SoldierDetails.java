import com.ms.wfc.core.*;
import com.ms.wfc.ui.*;

/**
 * This class is a visual component. The entry point for class execution
 * is the constructor.
 */
public class SoldierDetails extends UserControl
{
	public SoldierDetails()
	{
		super();

		// Required for Visual J++ Form Designer support
		//
		initForm();

				
		// add paint handler for mug shot
		mugs.addImage(new Bitmap("ruMugs.bmp"));
		mugs.addImage(new Bitmap("geMugs.bmp"));
		picture.addOnPaint(new PaintEventHandler(this.picture_paint));
	}
	
	
	static final String[] germanMedals = {
		"Knights Cross",
		"Iron Cross 1st Class",
		"Iron Cross 2nd Class",
		"War Merit Badge",
		"Assault Badge",
		"Wound Badge 3rd Class"
	};
	static final String[] russianMedals = {
		"Gold Star",
		"Kutuzov",
		"Red Banner",
		"Red Star",
		"Patriotic War 1st Class",
		"Patriotic War 2nd Class"
	};
	
/*	
none	55
one		133
two		161
three	197
four	233
five	255
six		282
seven	313
eight	349
nine	397
ten		>397
*/	
	
	static String getValueDescription(int value) {
		if (value < 55)
			return "Terrible";
		if (value < 133)
			return "Terrible";
		if (value < 161)
			return "Poor";
		if (value < 197)
			return "Poor";
		if (value < 233)
			return "Normal";
		if (value < 255)
			return "Normal";
		if (value < 282)
			return "Normal";
		if (value < 313)
			return "Good";
		if (value < 349)
			return "Good";
		if (value < 397)
			return "Excellent";
		return "Excellent";
	}
/*
1	Good
2	Injured
3	Incapacitated (with a red +)
4	Incapacitated (with a red X)
*/
	
	static Color getNameColor(int health) {
		if (health == 3 || health == 4) // incapacitated or dead
			return new Color(196,0,0);
		if (health == 2) // injured
			return new Color(196,196,0);
		return new Color(224,224,224);
	}
	
	static Color getHealthColor(int health) {
		if (health == 3 || health == 4) // incapacitated or dead
			return new Color(96,0,0);
		if (health == 2) // injured
			return new Color(96,96,0);
		return Color.WINDOWTEXT;
	}
	
	static String getHealthDescription(int value) {
		if (value == 1)
			return "Healthy";
		if (value == 2)
			return "Injured";
		if (value == 3)
			return "Incapacitated";
		if (value == 4)
			return "Dead";
		return "Unknown";
	}

	static void formatHealthDescription(Label label, int value) {
		label.setForeColor(getHealthColor(value));
		label.setText(getHealthDescription(value));
	}
	
	static void formatTextDescription(Label label, int value) {
		String description = getValueDescription(value);
		if (description.startsWith("Ter"))
			label.setForeColor(new Color(96,0,0));
		else if (description.startsWith("Ex"))
			label.setForeColor(new Color(0,96,0));
		else
			label.setForeColor(Color.WINDOWTEXT);
		
		label.setText(description);
	}
	
	static void formatTextValue(Label label, int value) {
		if (value <= 0)
			label.setText("");
		else
			label.setText(Integer.toString(value));
	}
	
	
	public void updateContent(TreeView tree, TreeNode n, SaveGame game) {
		
		side         = UnitEditor.getNodeData(tree,n.getParent().getParent());
		int teamIndex    = UnitEditor.getNodeData(tree,n.getParent());
		int soldierIndex = UnitEditor.getNodeData(tree, n);

		game.setSide(side != 0 ? true : false);
		
		imageIndex = (game.getSoldierName(soldierIndex).hashCode() & 0xffffff) % (11*7);
		picture.invalidate();
		
		// n contains the data for this team
		int teamMemberIndex = n.getIndex();
		int soldierClass = game.getTeamMemberClass(teamIndex, teamMemberIndex);
		name.setText(n.getText());
		description.setText(
			game.getTeamMemberTypeDescription(teamIndex, teamMemberIndex) + 
			" (" + game.getMasterSoldierWeaponName(soldierClass) + ")");
		
		weapons.removeAll();
		for (int i = 0; i < 4; i++) {
			int weaponClass = game.getMasterSoldierInt(soldierClass, game.MASTERSOLDIERS_CREWED_WEAPON + i*6);
			if (weaponClass > 0) {
				
				StringBuffer sb = new StringBuffer();
				
				sb.append(game.getMasterWeaponString(weaponClass, game.MASTERWEAPONS_NAME));
				
				// determine the amount of ammo a standard load out has
				int AP    = game.getMasterSoldierInt(soldierClass, game.MASTERSOLDIERS_CREWED_AP + i*6);
				int HE    = game.getMasterSoldierInt(soldierClass, game.MASTERSOLDIERS_CREWED_HE + i*6);
				int HEAT  = game.getMasterSoldierInt(soldierClass, game.MASTERSOLDIERS_CREWED_HEAT + i*6);
				int Smoke = game.getMasterSoldierInt(soldierClass, game.MASTERSOLDIERS_CREWED_SMOKE + i*6);
				int Spec  = game.getMasterSoldierInt(soldierClass, game.MASTERSOLDIERS_CREWED_SPEC + i*6);
				
				String seperator = "";
				sb.append(" (");
				if (AP > 0) 		{ sb.append(AP + " AP"); seperator = ", "; }
				if (HE > 0) 		{ sb.append(seperator + HE +  " HE"); seperator = ", "; }
				if (HEAT > 0) 		{ sb.append(seperator + HEAT + " HEAT"); seperator = ", "; }
				if (Smoke > 0) 		{ sb.append(seperator + Smoke + " Smoke"); seperator = ", "; }
				if (Spec > 0) 		{ sb.append(seperator + Spec + " Spec"); seperator = ", "; }
				sb.append(")");
				
				weapons.addItem(new String(sb));
			}
		}
		
		if (n.getText().startsWith("KIA")) {
			
			imageIndex = -1;
			picture.invalidate();
			
			name.setForeColor(new Color(128,0,0));
			
			leadership.setText("");
			morale.setText("");
			strength.setText("");
			intelligence.setText("");
			experience.setText("");
			health.setText("");
			
			abilities.removeAll();
			medals.removeAll();
			
			bravery.setText("");
			cowerdice.setText("");
			tanks.setText("");
			guns.setText("");
			vehicles.setText("");
			soldiers.setText("");
			
			return;
		}
		
		// format rank icon
		int rankIndex = game.getSoldierRankIndex(soldierIndex);
		Bitmap rankImage = (side != 0 ? germanRanks.getBitmap(rankIndex) : russianRanks.getBitmap(rankIndex));
		rankImage.setTransparentColor(Color.GREEN);
		rankImage.setTransparent(true);
		rank.setImage(rankImage);
	
		int healthValue = game.getSoldierHealth(soldierIndex);
		formatHealthDescription(health, healthValue);
		name.setForeColor(getNameColor(healthValue));
		
		formatTextDescription(leadership,	game.getSoldierLeadership(soldierIndex));
		formatTextDescription(morale,		game.getSoldierMorale(soldierIndex));
		formatTextDescription(strength,		game.getSoldierStrength(soldierIndex));
		formatTextDescription(intelligence,	game.getSoldierIntelligence(soldierIndex));
		formatTextDescription(experience,	game.getSoldierExperience(soldierIndex));

		formatTextValue(bravery,	game.getSoldierBravery(soldierIndex));
		formatTextValue(cowerdice,	game.getSoldierCowerdice(soldierIndex));
		formatTextValue(soldiers,	game.getSoldierSoldiers(soldierIndex));
		formatTextValue(tanks,		game.getSoldierTanks(soldierIndex));
		formatTextValue(vehicles,	game.getSoldierVehicles(soldierIndex));
		formatTextValue(guns,		game.getSoldierGuns(soldierIndex));
		
		// format medals
		medals.removeAll();
		for (int i = 0; i < 6; i++) {
			int medalCount = game.getSoldierMedal(soldierIndex, i);
			if (medalCount > 0) {
				medals.addItem(medalCount + " " + (side != 0 ? germanMedals[i] : russianMedals[i]));
			}
		}
		
		// format abilities
		abilities.removeAll();
		/*		
		CC				0
		Rifle			1
		Semiauto		2
		Sub-machine gun	3
		Anti-Tank		4
		Flame Thrower	5
		DemoCharge		6
		MG				7
		Mortar			8
		Large Gun		9
		Grenade			10
		Pistol			11
		Smk Mtr			12
		Light MG		13
		Incendiary		14
		*/	
		
		String[] ABILITY_DESCRIPTIONS = {
			"0.CC", 
			"1.Rifle", 
			"2.Semiauto", 
			"3.Sub-machine gun", 
			"4.Anti-Tank", 
			"5.Flame Thrower", 
			"6.Demo Charge", 
			"7.MG", 
			"8.Mortar", 
			"9.Large Gun", 
			"a.Grenade", 
			"b.Pistol", 
			"c.Smoke Mortar", 
			"d.Light MG", 
			"e.Molotov"
		};

		int offset = 52; // 52..60
		for (int i = 0; i < 8; i++) {
			int value = game.getSoldierByte(soldierIndex, offset);
			
			if (value > 0) {
				abilities.addItem(new String("offset:" + offset + " value:" + value));
				//abilities.addItem(ABILITY_DESCRIPTIONS[i] + " " + value);
			}
			offset += 1;
		}

		offset = game.SOLDIER_UNKNOWNSTART; // 102..144
		for (int i = 0; i < 42; i++) {
			int value = game.getSoldierByte(soldierIndex, offset);
			
			if (value > 0) {
				abilities.addItem(new String("offset:" + offset + " value:" + value));
				//abilities.addItem(ABILITY_DESCRIPTIONS[i] + " " + value);
			}
			offset += 1;
		}
	}
	
	int imageIndex = 0;
	int side = 0;

	private void picture_paint(Object source, PaintEvent e) {
		Graphics g = e.graphics;
		
		
		if (imageIndex < 0) {
			g.clearRect(e.clipRect);
			return;
		}

		Bitmap mugshot = mugs.getBitmap(side);
		
		int imagesPerRow = 11;
		int x = 52*(imageIndex % imagesPerRow);
		int y = 52*(imageIndex / imagesPerRow);
		
		Rectangle srcRect = new Rectangle(x,y,52,52);
		Rectangle dstRect = new Rectangle(0,0,52,52);
		g.drawImage(mugshot, srcRect, dstRect, false);
	}

	/**
	 * NOTE: The following code is required by the Visual J++ form
	 * designer.  It can be modified using the form editor.  Do not
	 * modify it using the code editor.
	 */
	Container components = new Container();
	Label label21 = new Label();
	Label name = new Label();
	Label label3 = new Label();
	Label label4 = new Label();
	Label label5 = new Label();
	Label label6 = new Label();
	Label label7 = new Label();
	Label label1a = new Label();
	PictureBox picture = new PictureBox();
	Label leadership = new Label();
	Label strength = new Label();
	Label intelligence = new Label();
	Label experience = new Label();
	Label morale = new Label();
	Label label2 = new Label();
	Label label13 = new Label();
	Label label14 = new Label();
	Label soldiers = new Label();
	Label tanks = new Label();
	Label vehicles = new Label();
	Label guns = new Label();
	Label description = new Label();
	Label label20 = new Label();
	ListBox abilities = new ListBox();
	ListBox weapons = new ListBox();
	Label label27 = new Label();
	Label label28 = new Label();
	Label bravery = new Label();
	Label cowerdice = new Label();
	Label label31 = new Label();
	ListBox medals = new ListBox();
	Label label22 = new Label();
	Label health = new Label();
	ImageList germanRanks = new ImageList();
	ImageList russianRanks = new ImageList();
	PictureBox rank = new PictureBox();
	ImageList mugs = new ImageList();

	private void initForm() { 
		// NOTE:  This form is storing resource information in an
		// external file.  Do not modify the string parameter to any
		// resources.getObject() function call. For example, do not
		// modify "foo1_location" in the following line of code
		// even if the name of the Foo object changes: 
		//   foo1.setLocation((Point)resources.getObject("foo1_location"));

		IResourceManager resources = new ResourceManager(this, "SoldierDetails");
		this.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		this.setSize(new Point(312, 406));
		this.setText("SoldierDetails");

		label21.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label21.setLocation(new Point(8, 64));
		label21.setSize(new Point(72, 16));
		label21.setTabIndex(26);
		label21.setTabStop(false);
		label21.setText("Weapons:");

		name.setAnchor(ControlAnchor.TOPLEFTRIGHT);
		name.setBackColor(Color.BLACK);
		name.setFont(new Font("Tahoma", 16.0f, FontSize.POINTS, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		name.setForeColor(new Color(224, 224, 224));
		name.setLocation(new Point(88, 0));
		name.setSize(new Point(216, 32));
		name.setTabIndex(0);
		name.setTabStop(false);
		name.setText("Capt Thor");

		label3.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label3.setLocation(new Point(160, 128));
		label3.setSize(new Point(72, 16));
		label3.setTabIndex(28);
		label3.setTabStop(false);
		label3.setText("Leadership:");

		label4.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label4.setLocation(new Point(160, 144));
		label4.setSize(new Point(72, 16));
		label4.setTabIndex(14);
		label4.setTabStop(false);
		label4.setText("Strength:");

		label5.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label5.setLocation(new Point(160, 160));
		label5.setSize(new Point(72, 16));
		label5.setTabIndex(10);
		label5.setTabStop(false);
		label5.setText("Intelligence:");

		label6.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label6.setLocation(new Point(8, 128));
		label6.setSize(new Point(72, 16));
		label6.setTabIndex(4);
		label6.setTabStop(false);
		label6.setText("Experience:");

		label7.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label7.setLocation(new Point(8, 144));
		label7.setSize(new Point(72, 16));
		label7.setTabIndex(1);
		label7.setTabStop(false);
		label7.setText("Morale:");

		label1a.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label1a.setLocation(new Point(8, 336));
		label1a.setSize(new Point(72, 16));
		label1a.setTabIndex(22);
		label1a.setTabStop(false);
		label1a.setText("Soldiers:");

		picture.setLocation(new Point(8, 0));
		picture.setSize(new Point(56, 56));
		picture.setTabIndex(35);
		picture.setTabStop(false);
		picture.setText("pictureBox1");
		picture.setBorderStyle(BorderStyle.FIXED_3D);

		leadership.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		leadership.setForeColor(new Color(0, 128, 0));
		leadership.setLocation(new Point(240, 128));
		leadership.setSize(new Point(80, 16));
		leadership.setTabIndex(21);
		leadership.setTabStop(false);
		leadership.setText("Excellent");

		strength.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		strength.setLocation(new Point(240, 144));
		strength.setSize(new Point(80, 16));
		strength.setTabIndex(19);
		strength.setTabStop(false);
		strength.setText("Good");

		intelligence.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		intelligence.setLocation(new Point(240, 160));
		intelligence.setSize(new Point(80, 16));
		intelligence.setTabIndex(18);
		intelligence.setTabStop(false);
		intelligence.setText("Normal");

		experience.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		experience.setForeColor(Color.WINDOWTEXT);
		experience.setLocation(new Point(88, 128));
		experience.setSize(new Point(80, 16));
		experience.setTabIndex(17);
		experience.setTabStop(false);
		experience.setText("Poor");

		morale.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		morale.setForeColor(new Color(128, 0, 0));
		morale.setLocation(new Point(88, 144));
		morale.setSize(new Point(80, 16));
		morale.setTabIndex(16);
		morale.setTabStop(false);
		morale.setText("Terrible");

		label2.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label2.setLocation(new Point(8, 352));
		label2.setSize(new Point(72, 16));
		label2.setTabIndex(12);
		label2.setTabStop(false);
		label2.setText("Tanks:");

		label13.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label13.setLocation(new Point(8, 368));
		label13.setSize(new Point(72, 16));
		label13.setTabIndex(7);
		label13.setTabStop(false);
		label13.setText("Vehicles:");

		label14.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label14.setLocation(new Point(8, 384));
		label14.setSize(new Point(72, 16));
		label14.setTabIndex(3);
		label14.setTabStop(false);
		label14.setText("Guns:");

		soldiers.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		soldiers.setLocation(new Point(88, 336));
		soldiers.setSize(new Point(64, 16));
		soldiers.setTabIndex(15);
		soldiers.setTabStop(false);
		soldiers.setText("4");

		tanks.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		tanks.setLocation(new Point(88, 352));
		tanks.setSize(new Point(64, 16));
		tanks.setTabIndex(13);
		tanks.setTabStop(false);
		tanks.setText("1");

		vehicles.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		vehicles.setLocation(new Point(88, 368));
		vehicles.setSize(new Point(64, 16));
		vehicles.setTabIndex(11);
		vehicles.setTabStop(false);
		vehicles.setText(" ");

		guns.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		guns.setLocation(new Point(88, 384));
		guns.setSize(new Point(64, 16));
		guns.setTabIndex(9);
		guns.setTabStop(false);
		guns.setText(" ");

		description.setAnchor(ControlAnchor.TOPLEFTRIGHT);
		description.setBackColor(Color.BLACK);
		description.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		description.setForeColor(new Color(224, 224, 224));
		description.setLocation(new Point(72, 32));
		description.setSize(new Point(232, 24));
		description.setTabIndex(23);
		description.setTabStop(false);
		description.setText("Alpha 12 Leader");

		label20.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label20.setLocation(new Point(8, 192));
		label20.setSize(new Point(296, 16));
		label20.setTabIndex(24);
		label20.setTabStop(false);
		label20.setText("Unknown Values (bytes 52..60 && 102..144):");

		abilities.setAnchor(ControlAnchor.TOPLEFTRIGHT);
		abilities.setBackColor(Color.CONTROL);
		abilities.setLocation(new Point(8, 208));
		abilities.setSize(new Point(296, 65));
		abilities.setTabIndex(29);
		abilities.setText("listBox1");
		abilities.setBorderStyle(BorderStyle.NONE);
		abilities.setColumnWidth(151);
		abilities.setMultiColumn(true);
		abilities.setSelectionMode(SelectionMode.NONE);
		abilities.setUseTabStops(true);
		abilities.setItems(new Object[] {
						   "Excellent Rifle", 
						   "Good Grenade", 
						   "Good Hand to Hand", 
						   "Poor AT", 
						   "Terrible Large Gun", 
						   "Terrible Mortar"});

		weapons.setAnchor(ControlAnchor.TOPLEFTRIGHT);
		weapons.setBackColor(Color.CONTROL);
		weapons.setLocation(new Point(8, 80));
		weapons.setSize(new Point(296, 26));
		weapons.setTabIndex(31);
		weapons.setText("listBox1");
		weapons.setBorderStyle(BorderStyle.NONE);
		weapons.setColumnWidth(150);
		weapons.setMultiColumn(true);
		weapons.setSelectionMode(SelectionMode.NONE);
		weapons.setUseTabStops(true);
		weapons.setItems(new Object[] {
						 "MP40 (7 AP)", 
						 "Grenade (2 AP, 1 Smoke)"});

		label27.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label27.setLocation(new Point(8, 296));
		label27.setSize(new Point(72, 16));
		label27.setTabIndex(6);
		label27.setTabStop(false);
		label27.setText("Bravery:");

		label28.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label28.setLocation(new Point(8, 312));
		label28.setSize(new Point(72, 16));
		label28.setTabIndex(2);
		label28.setTabStop(false);
		label28.setText("Cowerdice:");

		bravery.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		bravery.setLocation(new Point(88, 296));
		bravery.setSize(new Point(64, 16));
		bravery.setTabIndex(8);
		bravery.setTabStop(false);
		bravery.setText("2");

		cowerdice.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		cowerdice.setLocation(new Point(88, 312));
		cowerdice.setSize(new Point(64, 16));
		cowerdice.setTabIndex(5);
		cowerdice.setTabStop(false);
		cowerdice.setText(" ");

		label31.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label31.setLocation(new Point(160, 296));
		label31.setSize(new Point(144, 16));
		label31.setTabIndex(25);
		label31.setTabStop(false);
		label31.setText("Decorations:");

		medals.setAnchor(ControlAnchor.TOPLEFTRIGHT);
		medals.setBackColor(Color.CONTROL);
		medals.setLocation(new Point(160, 312));
		medals.setSize(new Point(144, 78));
		medals.setTabIndex(30);
		medals.setText("listBox1");
		medals.setBorderStyle(BorderStyle.NONE);
		medals.setSelectionMode(SelectionMode.NONE);
		medals.setUseTabStops(true);
		medals.setItems(new Object[] {
						"Iron Cross", 
						"Assualt Badge (x2)", 
						"Wound Badage"});

		label22.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label22.setLocation(new Point(8, 160));
		label22.setSize(new Point(72, 16));
		label22.setTabIndex(27);
		label22.setTabStop(false);
		label22.setText("Health:");

		health.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		health.setForeColor(Color.WINDOWTEXT);
		health.setLocation(new Point(88, 160));
		health.setSize(new Point(80, 16));
		health.setTabIndex(20);
		health.setTabStop(false);
		health.setText("Normal");

		germanRanks.setImageSize(new Point(32, 32));
		germanRanks.setImageStream((ImageListStreamer)resources.getObject("germanRanks_imageStream"));
		/* @designTimeOnly germanRanks.setLocation(new Point(208, 56)); */

		russianRanks.setImageSize(new Point(32, 32));
		russianRanks.setImageStream((ImageListStreamer)resources.getObject("russianRanks_imageStream"));
		/* @designTimeOnly russianRanks.setLocation(new Point(208, 80)); */

		rank.setBackColor(Color.BLACK);
		rank.setLocation(new Point(72, 0));
		rank.setSize(new Point(16, 32));
		rank.setTabIndex(36);
		rank.setTabStop(false);
		rank.setText("pictureBox1");
		rank.setImage((Bitmap)resources.getObject("rank_image"));
		rank.setSizeMode(PictureBoxSizeMode.CENTER_IMAGE);

		mugs.setImageSize(new Point(624, 364));
		mugs.setUseMask(false);
		/* @designTimeOnly mugs.setLocation(new Point(208, 104)); */

		this.setNewControls(new Control[] {
							picture, 
							rank, 
							label5, 
							label4, 
							label3, 
							health, 
							label22, 
							label20, 
							abilities, 
							medals, 
							label31, 
							cowerdice, 
							bravery, 
							label28, 
							label27, 
							weapons, 
							label21, 
							description, 
							guns, 
							vehicles, 
							tanks, 
							label14, 
							label13, 
							label2, 
							label1a, 
							morale, 
							experience, 
							intelligence, 
							strength, 
							leadership, 
							label7, 
							label6, 
							name, 
							soldiers});
	}

	public static class ClassInfo extends UserControl.ClassInfo
	{
		// TODO: Add your property and event infos here
	}
}
