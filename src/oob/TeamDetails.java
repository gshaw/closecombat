import com.ms.wfc.core.*;
import com.ms.wfc.ui.*;

/**
 * This class is a visual component. The entry point for class execution
 * is the constructor.
 */
public class TeamDetails extends UserControl
{
	public TeamDetails()
	{
		super();

		// Required for Visual J++ Form Designer support
		//
		initForm();

		Bitmap img = (Bitmap)icon.getImage();
		img.setTransparent(true);
		img.setTransparentColor(Color.WHITE);
	}
/*	
	
<    Morale				Experience
---- ------------------ ------------------
<162 Likely to Run		No Training
<214 Low Morale			Basic Training
<256 Fair Morale		Seen Battle
<298 Good Morale		Experienced
<350 Excellent Morale	Tough Veterans
>350 Fight to the death	Shock Troops!	
	
*/
	
	
	
	static String getMoraleDescription(int morale) {
		if (morale <= 162)
			return "Likely to Run";
		if (morale <= 214)
			return "Low Morale";
		if (morale <= 256)
			return "Fair Morale";
		if (morale <= 298)
			return "Good Morale";
		if (morale <= 350)
			return "Excellent Morale";
		return "Fight to the death";
	}
	
	static String getExperienceDescription(int experience) {
		if (experience <= 162)
			return "No Training";
		if (experience <= 214)
			return "Basic Training";
		if (experience <= 256)
			return "Seen Battle";
		if (experience <= 298)
			return "Experienced";
		if (experience <= 350)
			return "Tough Veterans";
		return "Shock Troops!";
	}
	
	public void updateContent(TreeView tree, TreeNode n, SaveGame game) {
		
		int side = UnitEditor.getNodeData(tree,n.getParent());
		game.setSide(side != 0 ? true : false);
		
		int teamIndex = UnitEditor.getNodeData(tree,n);
			
		
		// n contains the data for this team
		name.setText(n.getText());
		
		description.setText(game.getTeamDescription(teamIndex));
		morale.setText(getMoraleDescription(game.getTeamMorale(teamIndex)));
		experience.setText(getExperienceDescription(game.getTeamExperience(teamIndex)));
		
		soldiers.removeAll();
		int soldierCount = n.getNodeCount();
		for (int i = 0; i < soldierCount; i++) {
			soldiers.addItem(n.getNode(i).getText());
		}
	}

	/**
	 * NOTE: The following code is required by the Visual J++ form
	 * designer.  It can be modified using the form editor.  Do not
	 * modify it using the code editor.
	 */
	Container components = new Container();
	Label name = new Label();
	Label description = new Label();
	PictureBox icon = new PictureBox();
	Label label21 = new Label();
	ListBox soldiers = new ListBox();
	Label label6 = new Label();
	Label label7 = new Label();
	Label experience = new Label();
	Label morale = new Label();

	private void initForm() { 
		// NOTE:  This form is storing resource information in an
		// external file.  Do not modify the string parameter to any
		// resources.getObject() function call. For example, do not
		// modify "foo1_location" in the following line of code
		// even if the name of the Foo object changes: 
		//   foo1.setLocation((Point)resources.getObject("foo1_location"));

		IResourceManager resources = new ResourceManager(this, "TeamDetails");
		this.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		this.setSize(new Point(312, 406));
		this.setText("TeamDetails");

		name.setFont(new Font("Tahoma", 16.0f, FontSize.POINTS, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		name.setForeColor(Color.WINDOWTEXT);
		name.setLocation(new Point(72, 0));
		name.setSize(new Point(232, 32));
		name.setTabIndex(0);
		name.setTabStop(false);
		name.setText("Team Name");

		description.setAnchor(ControlAnchor.TOPLEFTRIGHT);
		description.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		description.setLocation(new Point(72, 32));
		description.setSize(new Point(232, 16));
		description.setTabIndex(2);
		description.setTabStop(false);
		description.setText(" Schutzen (1942) ");

		icon.setBackColor(Color.BLACK);
		icon.setLocation(new Point(8, 0));
		icon.setSize(new Point(56, 56));
		icon.setTabIndex(6);
		icon.setTabStop(false);
		icon.setText("pictureBox1");
		icon.setBorderStyle(BorderStyle.FIXED_3D);
		icon.setImage((Bitmap)resources.getObject("icon_image"));
		icon.setSizeMode(PictureBoxSizeMode.CENTER_IMAGE);

		label21.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label21.setLocation(new Point(8, 112));
		label21.setSize(new Point(72, 16));
		label21.setTabIndex(3);
		label21.setTabStop(false);
		label21.setText("Soldiers:");

		soldiers.setBackColor(Color.CONTROL);
		soldiers.setLocation(new Point(8, 128));
		soldiers.setSize(new Point(296, 130));
		soldiers.setTabIndex(4);
		soldiers.setText("listBox1");
		soldiers.setBorderStyle(BorderStyle.NONE);
		soldiers.setSelectionMode(SelectionMode.NONE);
		soldiers.setUseTabStops(true);
		soldiers.setItems(new Object[] {
						  "Leader", 
						  "Soldat 2", 
						  "Soldat 3", 
						  "Soldat 4", 
						  "Soldat 5", 
						  "Soldat 6", 
						  "Soldat 7", 
						  "Soldat 8", 
						  "Soldat 9", 
						  "Soldat 10"});

		label6.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label6.setLocation(new Point(8, 80));
		label6.setSize(new Point(72, 16));
		label6.setTabIndex(5);
		label6.setTabStop(false);
		label6.setText("Experience:");

		label7.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label7.setLocation(new Point(8, 64));
		label7.setSize(new Point(72, 16));
		label7.setTabIndex(1);
		label7.setTabStop(false);
		label7.setText("Morale:");

		experience.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		experience.setForeColor(Color.WINDOWTEXT);
		experience.setLocation(new Point(88, 80));
		experience.setSize(new Point(104, 16));
		experience.setTabIndex(7);
		experience.setTabStop(false);
		experience.setText("Poor");

		morale.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		morale.setForeColor(Color.WINDOWTEXT);
		morale.setLocation(new Point(88, 64));
		morale.setSize(new Point(104, 16));
		morale.setTabIndex(8);
		morale.setTabStop(false);
		morale.setText("Terrible");

		this.setNewControls(new Control[] {
							morale, 
							experience, 
							label7, 
							label6, 
							icon, 
							soldiers, 
							label21, 
							description, 
							name});
	}

	public static class ClassInfo extends UserControl.ClassInfo
	{
		// TODO: Add your property and event infos here
	}
}
