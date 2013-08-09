import com.ms.wfc.core.*;
import com.ms.wfc.ui.*;

/**
 * This class is a visual component. The entry point for class execution
 * is the constructor.
 */
public class UnitDetails extends UserControl
{
	public UnitDetails()
	{
		super();

		// Required for Visual J++ Form Designer support
		//
		initForm();

		// TODO: Add any constructor code after initForm call
	}
	
	public void updateContent(TreeView tree, TreeNode n, SaveGame game) {
		int side = UnitEditor.getNodeData(tree,n);
		russianIcon.setVisible(side == 0);
		germanIcon.setVisible(side == 1);
		
		// n contains the data for this team
		name.setText(n.getText());
		
		points.setText("unknown");
		report.removeAll();
	}

	/**
	 * NOTE: The following code is required by the Visual J++ form
	 * designer.  It can be modified using the form editor.  Do not
	 * modify it using the code editor.
	 */
	Container components = new Container();
	Label label2 = new Label();
	PictureBox germanIcon = new PictureBox();
	ListBox report = new ListBox();
	Edit name = new Edit();
	Label points = new Label();
	Label label6 = new Label();
	PictureBox russianIcon = new PictureBox();

	private void initForm() { 
		// NOTE:  This form is storing resource information in an
		// external file.  Do not modify the string parameter to any
		// resources.getObject() function call. For example, do not
		// modify "foo1_location" in the following line of code
		// even if the name of the Foo object changes: 
		//   foo1.setLocation((Point)resources.getObject("foo1_location"));

		IResourceManager resources = new ResourceManager(this, "UnitDetails");
		this.setBackColor(Color.CONTROL);
		this.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		this.setForeColor(Color.WINDOWTEXT);
		this.setSize(new Point(312, 406));
		this.setText("UnitDetails");

		label2.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label2.setForeColor(Color.WINDOWTEXT);
		label2.setLocation(new Point(8, 64));
		label2.setSize(new Point(88, 16));
		label2.setTabIndex(3);
		label2.setTabStop(false);
		label2.setText("Req Points:");

		germanIcon.setBackColor(Color.BLACK);
		germanIcon.setLocation(new Point(8, 0));
		germanIcon.setSize(new Point(56, 56));
		germanIcon.setTabIndex(0);
		germanIcon.setTabStop(false);
		germanIcon.setText("pictureBox1");
		germanIcon.setVisible(false);
		germanIcon.setBorderStyle(BorderStyle.FIXED_3D);
		germanIcon.setImage((Bitmap)resources.getObject("germanIcon_image"));
		germanIcon.setSizeMode(PictureBoxSizeMode.CENTER_IMAGE);

		report.setBackColor(Color.CONTROL);
		report.setForeColor(Color.BLACK);
		report.setLocation(new Point(8, 104));
		report.setSize(new Point(296, 286));
		report.setTabIndex(5);
		report.setText("listBox1");
		report.setBorderStyle(BorderStyle.NONE);
		report.setSelectionMode(SelectionMode.NONE);
		report.setUseTabStops(true);
		report.setItems(new Object[] {
						"Pvt Smith (Alpha 11) is kill leader (14)", 
						"Cpl Green (Alpha 41) has 11 kills", 
						"Alpha 41 needs replacements (<25% strength)"});

		name.setBackColor(Color.CONTROL);
		name.setFont(new Font("Tahoma", 16.0f, FontSize.POINTS, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		name.setForeColor(Color.WINDOWTEXT);
		name.setLocation(new Point(72, 0));
		name.setSize(new Point(232, 32));
		name.setTabIndex(4);
		name.setText("Germans/Russians");
		name.setBorderStyle(BorderStyle.NONE);

		points.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.NORMAL, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		points.setForeColor(Color.WINDOWTEXT);
		points.setLocation(new Point(104, 64));
		points.setSize(new Point(104, 16));
		points.setTabIndex(1);
		points.setTabStop(false);
		points.setText("45");

		label6.setFont(new Font("Tahoma", 11.0f, FontSize.CHARACTERHEIGHT, FontWeight.BOLD, false, false, false, com.ms.wfc.app.CharacterSet.DEFAULT, 0));
		label6.setForeColor(Color.WINDOWTEXT);
		label6.setLocation(new Point(8, 88));
		label6.setSize(new Point(88, 16));
		label6.setTabIndex(2);
		label6.setTabStop(false);
		label6.setText("Unit Report:");

		russianIcon.setBackColor(Color.BLACK);
		russianIcon.setLocation(new Point(8, 0));
		russianIcon.setSize(new Point(56, 56));
		russianIcon.setTabIndex(6);
		russianIcon.setTabStop(false);
		russianIcon.setText("pictureBox1");
		russianIcon.setVisible(false);
		russianIcon.setBorderStyle(BorderStyle.FIXED_3D);
		russianIcon.setImage((Bitmap)resources.getObject("russianIcon_image"));
		russianIcon.setSizeMode(PictureBoxSizeMode.CENTER_IMAGE);

		this.setNewControls(new Control[] {
							russianIcon, 
							report, 
							label6, 
							name, 
							label2, 
							points, 
							germanIcon});
	}

	public static class ClassInfo extends UserControl.ClassInfo
	{
		// TODO: Add your property and event infos here
	}
}
