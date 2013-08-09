import com.ms.wfc.app.*;
import com.ms.wfc.core.*;
import com.ms.wfc.ui.*;
import com.ms.wfc.html.*;
import com.ms.win32.*;
import com.ms.win32.Comctl32.*;

/**
 * This class can take a variable number of parameters on the command
 * line. Program execution begins with the main() method. The class
 * constructor is not invoked unless an object of type 'UnitEditor'
 * created in the main() method.
 */
public class UnitEditor extends Form { 
	
	SaveGame game;
	
	public UnitEditor()
	{
		super();
		
		// Required for Visual J++ Form Designer support
		initForm();
		
		updateSaveFile("no file loaded");
	}
	
	void updateSaveFile(String pathName) {
		
		setText("Order of Battle [" + pathName + "]");

		unit.removeAll();
		
		game = new SaveGame(pathName);

		TreeNode russians = new TreeNode("Russians");
		game.setSide(game.RUSSIANS);
		updateRootNode(russians);
		unit.addNode(russians);

		TreeNode germans = new TreeNode("Germans");
		game.setSide(game.GERMANS);
		updateRootNode(germans);
		unit.addNode(germans);

	}

	static void setNodeData(TreeView tree, TreeNode n, int data) {
		n.setImageIndex(data);
		/*
		TV_ITEM item = new TV_ITEM();
		item.hItem = n.getHandle();
		item.mask = com.ms.win32.wint.TVIF_PARAM;
		com.ms.win32.Comctl32.TreeView_GetItem(tree.getHandle(), item);
		item.lParam = data;
		com.ms.win32.Comctl32.TreeView_SetItem(tree.getHandle(), item);
		*/
	}
	
	static int getNodeData(TreeView tree, TreeNode n) {
		return n.getImageIndex();
		/*
		TV_ITEM item = new TV_ITEM();
		com.ms.win32.Comctl32.TreeView_GetItem(tree.getHandle(), item);
		return item.lParam;
		*/
	}
	
	
	private void updateRootNode(TreeNode organization) {
		setNodeData(unit, organization, game.getSide() ? 1 : 0);
		for (int teamIndex = 0; teamIndex < game.TEAM_COUNT; teamIndex++) {
			if (game.getTeamType(teamIndex) > 0) {
				TreeNode team = new TreeNode(game.getTeamName(teamIndex));
				setNodeData(unit, team, teamIndex);
				
				int teamMemberCount = game.getTeamMemberCount(teamIndex);
				for (int teamMember = 0; teamMember < teamMemberCount; teamMember++) {
					int soldierIndex = game.getTeamSoldierIndex(teamIndex, teamMember);
					
					String soldierTitle = "KIA/MIA";
					if (soldierIndex >= 0) {
						soldierTitle = game.getSoldierRank(soldierIndex) + game.getSoldierName(soldierIndex);
					}
					TreeNode soldier = new TreeNode(soldierTitle);
					setNodeData(unit, soldier, soldierIndex);
					
					team.addNode(soldier);
				}
				
				organization.addNode(team);
			}
		}
	}
	
	/**
	 * UnitEditor overrides dispose so it can clean up the
	 * component list.
	 */
	public void dispose()
	{
		super.dispose();
		components.dispose();
	}

	static final int UNIT = 1;
	static final int TEAM = 2;
	static final int SOLDIER = 3;
		
	protected static int getNodeType(TreeNode n) {
		TreeNode parent = n.getParent();
		if (parent != null) {
			TreeNode grandParent = parent.getParent();
			if (grandParent != null) {
				return SOLDIER;
			}
			return TEAM;
		}
		return UNIT;
	}
	
	private void unit_afterSelect(Object source, TreeViewEvent e) {
		int nodeType = getNodeType(e.node);
		
		switch (nodeType) {
		case UNIT:
			unitDetails.setVisible(true);
			teamDetails.setVisible(false);
			soldierDetails.setVisible(false);
			
			unitDetails.updateContent(unit, e.node, game);
			break;
			
		case TEAM:
			unitDetails.setVisible(false);
			teamDetails.setVisible(true);
			soldierDetails.setVisible(false);

			teamDetails.updateContent(unit, e.node, game);
			break;
			
		case SOLDIER:
			unitDetails.setVisible(false);
			teamDetails.setVisible(false);
			soldierDetails.setVisible(true);
			
			soldierDetails.updateContent(unit, e.node, game);
			break;
		}
	}

	private void menuOpen_click(Object source, Event e) {
		if (openFileDialog.showDialog() == DialogResult.OK) {
			updateSaveFile(openFileDialog.getFileName());
		}
	}

	private void menuExit_click(Object source, Event e) {
		Application.exit();
	}

	/**
	 * NOTE: The following code is required by the Visual J++ form
	 * designer.  It can be modified using the form editor.  Do not
	 * modify it using the code editor.
	 */
	Container components = new Container();
	TreeView unit = new TreeView();
	TeamDetails teamDetails = new TeamDetails();
	UnitDetails unitDetails = new UnitDetails();
	Panel details = new Panel();
	SoldierDetails soldierDetails = new SoldierDetails();
	MenuItem menuFile = new MenuItem();
	MainMenu mainMenu = new MainMenu();
	OpenFileDialog openFileDialog = new OpenFileDialog();
	MenuItem menuOpen = new MenuItem();
	MenuItem menuItem2 = new MenuItem();
	MenuItem menuExit = new MenuItem();

	private void initForm() { 
		// NOTE:  This form is storing resource information in an
		// external file.  Do not modify the string parameter to any
		// resources.getObject() function call. For example, do not
		// modify "foo1_location" in the following line of code
		// even if the name of the Foo object changes: 
		//   foo1.setLocation((Point)resources.getObject("foo1_location"));

		IResourceManager resources = new ResourceManager(this, "UnitEditor");
		unit.setLocation(new Point(8, 8));
		unit.setSize(new Point(248, 422));
		unit.setTabIndex(0);
		unit.setText("treeView1");
		unit.setIndent(19);
		unit.setNodes(new TreeNode[] {
					  new TreeNode("Germans"), 
					  new TreeNode("Russians")});
		unit.addOnAfterSelect(new TreeViewEventHandler(this.unit_afterSelect));

		teamDetails.setDock(ControlDock.FILL);
		teamDetails.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, CharacterSet.DEFAULT, 0));
		teamDetails.setSize(new Point(312, 424));
		teamDetails.setTabIndex(0);
		teamDetails.setText("teamDetails1");
		teamDetails.setVisible(false);

		unitDetails.setBackColor(Color.CONTROL);
		unitDetails.setDock(ControlDock.FILL);
		unitDetails.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, CharacterSet.DEFAULT, 0));
		unitDetails.setForeColor(new Color(0, 0, 64));
		unitDetails.setSize(new Point(312, 424));
		unitDetails.setTabIndex(1);
		unitDetails.setText("unitDetails1");
		unitDetails.setVisible(false);

		details.setLocation(new Point(256, 8));
		details.setSize(new Point(312, 424));
		details.setTabIndex(1);
		details.setText("panel1");

		soldierDetails.setDock(ControlDock.FILL);
		soldierDetails.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, CharacterSet.DEFAULT, 0));
		soldierDetails.setSize(new Point(312, 424));
		soldierDetails.setTabIndex(2);
		soldierDetails.setText("soldierDetails");

		openFileDialog.setFilter("All Files|*.*");
		openFileDialog.setInitialDir("c:\\games\\cc3\\games\\save");
		/* @designTimeOnly openFileDialog.setLocation(new Point(128, 400)); */

		menuOpen.setShortcut(Shortcut.CTRL_O);
		menuOpen.setText("&Open...");
		menuOpen.addOnClick(new EventHandler(this.menuOpen_click));

		menuItem2.setText("-");

		menuExit.setText("E&xit");
		menuExit.addOnClick(new EventHandler(this.menuExit_click));

		menuFile.setMenuItems(new MenuItem[] {
							  menuOpen, 
							  menuItem2, 
							  menuExit});
		menuFile.setText("&File");

		mainMenu.setMenuItems(new MenuItem[] {
							  menuFile});
		/* @designTimeOnly mainMenu.setLocation(new Point(16, 392)); */

		this.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, CharacterSet.DEFAULT, 0));
		this.setText("Order of Battle [no file loaded]");
		this.setAutoScaleBaseSize(new Point(5, 13));
		this.setBorderStyle(FormBorderStyle.FIXED_DIALOG);
		this.setClientSize(new Point(570, 438));
		this.setIcon((Icon)resources.getObject("this_icon"));
		this.setMaximizeBox(false);
		this.setMenu(mainMenu);
		this.setMinimizeBox(false);

		this.setNewControls(new Control[] {
							unit, 
							details});
		details.setNewControls(new Control[] {
							   soldierDetails, 
							   teamDetails, 
							   unitDetails});
	}

	/**
	 * The main entry point for the application. 
	 *
	 * @param args Array of parameters passed to the application
	 * via the command line.
	 */
	public static void main(String args[])
	{
		Application.run(new UnitEditor());
	}
}
