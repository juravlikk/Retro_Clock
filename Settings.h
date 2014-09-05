#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

extern int WND_H;
extern int WND_W;
extern int RButton;
extern int WindowVisIndex;

extern bool SetS;
extern bool AutorunChecked;
extern bool TrayChecked;
extern bool TaskbarChecked;
extern bool R[5];

extern void AutorunADD();
extern void AutorunDEL();
extern void AddToTray();
extern void DelFromTray();
extern void HideIconT();
extern void ShowIconT();
extern void TranslaterS(String ^s);
extern void WindowVisibility(int index);
extern void Reset();
extern bool Compare(String ^s);

namespace Vertices {

	/// <summary>
	/// Summary for Settings
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Settings : public System::Windows::Forms::Form
	{
	private:
		static Settings ^set = gcnew Settings();
	public: static Settings^ GetSet()
			{
				return set;
			}
	public:
		Settings()
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Settings()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::Button^  button2;
	public: System::Windows::Forms::CheckBox^  checkBox1;
	public: System::Windows::Forms::CheckBox^  checkBox2;
	public: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
	public: 
	public: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::Button^  button3;
	public: System::Windows::Forms::RadioButton^  radioButton1;
	public: System::Windows::Forms::RadioButton^  radioButton2;
	public: System::Windows::Forms::RadioButton^  radioButton3;
	public: System::Windows::Forms::RadioButton^  radioButton4;
	public: System::Windows::Forms::RadioButton^  radioButton5;
	public: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::HScrollBar^  hScrollBar1;
	public: System::Windows::Forms::Label^  label4;
	private: 
	public: System::Windows::Forms::Label^  label5;
	public: 

	public: 

	public: 

	public: 

	public: 

	public: 
	private: 
	public: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Settings::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton4 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton5 = (gcnew System::Windows::Forms::RadioButton());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Enabled = false;
			this->button1->Location = System::Drawing::Point(179, 252);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(62, 23);
			this->button1->TabIndex = 0;
			this->button1->Text = L"OK";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Settings::button1_Click);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(247, 252);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(57, 23);
			this->button2->TabIndex = 1;
			this->button2->Text = L"Cancel";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Settings::button2_Click);
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(17, 35);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(110, 17);
			this->checkBox1->TabIndex = 2;
			this->checkBox1->Text = L"Show on Taskbar";
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Settings::checkBox1_CheckedChanged);
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(17, 58);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(81, 17);
			this->checkBox2->TabIndex = 3;
			this->checkBox2->Text = L"Add to Tray";
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &Settings::checkBox2_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(17, 81);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(97, 17);
			this->checkBox3->TabIndex = 4;
			this->checkBox3->Text = L"Add to Autorun";
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &Settings::checkBox3_CheckedChanged);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(9, 205);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(237, 20);
			this->textBox1->TabIndex = 5;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &Settings::textBox1_TextChanged);
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(252, 203);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(52, 23);
			this->button3->TabIndex = 6;
			this->button3->Text = L"Browse";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Settings::button3_Click);
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Location = System::Drawing::Point(160, 34);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(74, 17);
			this->radioButton1->TabIndex = 7;
			this->radioButton1->Tag = L"1";
			this->radioButton1->Text = L"Very Small";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &Settings::radioButton1_CheckedChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(160, 57);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(50, 17);
			this->radioButton2->TabIndex = 8;
			this->radioButton2->Tag = L"2";
			this->radioButton2->Text = L"Small";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &Settings::radioButton2_CheckedChanged);
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Checked = true;
			this->radioButton3->Location = System::Drawing::Point(160, 81);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(62, 17);
			this->radioButton3->TabIndex = 9;
			this->radioButton3->TabStop = true;
			this->radioButton3->Tag = L"3";
			this->radioButton3->Text = L"Medium";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &Settings::radioButton3_CheckedChanged);
			// 
			// radioButton4
			// 
			this->radioButton4->AutoSize = true;
			this->radioButton4->Location = System::Drawing::Point(240, 34);
			this->radioButton4->Name = L"radioButton4";
			this->radioButton4->Size = System::Drawing::Size(40, 17);
			this->radioButton4->TabIndex = 10;
			this->radioButton4->Tag = L"4";
			this->radioButton4->Text = L"Big";
			this->radioButton4->UseVisualStyleBackColor = true;
			this->radioButton4->CheckedChanged += gcnew System::EventHandler(this, &Settings::radioButton4_CheckedChanged);
			// 
			// radioButton5
			// 
			this->radioButton5->AutoSize = true;
			this->radioButton5->Location = System::Drawing::Point(240, 57);
			this->radioButton5->Name = L"radioButton5";
			this->radioButton5->Size = System::Drawing::Size(64, 17);
			this->radioButton5->TabIndex = 11;
			this->radioButton5->Tag = L"5";
			this->radioButton5->Text = L"Very Big";
			this->radioButton5->UseVisualStyleBackColor = true;
			this->radioButton5->CheckedChanged += gcnew System::EventHandler(this, &Settings::radioButton5_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(201, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(40, 20);
			this->label1->TabIndex = 12;
			this->label1->Text = L"Size";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label2->Location = System::Drawing::Point(12, 173);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(114, 20);
			this->label2->TabIndex = 13;
			this->label2->Text = L"Textures folder";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label3->Location = System::Drawing::Point(13, 9);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(127, 20);
			this->label3->TabIndex = 14;
			this->label3->Text = L"Visual properties";
			// 
			// hScrollBar1
			// 
			this->hScrollBar1->LargeChange = 1;
			this->hScrollBar1->Location = System::Drawing::Point(9, 140);
			this->hScrollBar1->Name = L"hScrollBar1";
			this->hScrollBar1->Size = System::Drawing::Size(295, 17);
			this->hScrollBar1->TabIndex = 15;
			this->hScrollBar1->Value = 100;
			this->hScrollBar1->ValueChanged += gcnew System::EventHandler(this, &Settings::hScrollBar1_ValueChanged);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label4->Location = System::Drawing::Point(13, 110);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(107, 20);
			this->label4->TabIndex = 16;
			this->label4->Text = L"Transparance";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(204)));
			this->label5->Location = System::Drawing::Point(126, 110);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(32, 20);
			this->label5->TabIndex = 17;
			this->label5->Text = L"0%";
			// 
			// Settings
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(316, 287);
			this->ControlBox = false;
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->hScrollBar1);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->radioButton5);
			this->Controls->Add(this->radioButton4);
			this->Controls->Add(this->radioButton3);
			this->Controls->Add(this->radioButton2);
			this->Controls->Add(this->radioButton1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->checkBox3);
			this->Controls->Add(this->checkBox2);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"Settings";
			this->SizeGripStyle = System::Windows::Forms::SizeGripStyle::Hide;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Settings";
			this->Shown += gcnew System::EventHandler(this, &Settings::Settings_Shown);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 
				 if ((!Compare(this->textBox1->Text)) | (this->radioButton1->Checked != R[0]) |  (this->radioButton2->Checked != R[1]) | (this->radioButton3->Checked != R[2]) | (this->radioButton4->Checked != R[3]) | (this->radioButton5->Checked != R[4])) {
					 if((MessageBox::Show(L"You must restart program to save changes. Are you really want to do this?",L"Save changes", MessageBoxButtons::YesNo, MessageBoxIcon::Question) == ::DialogResult::Yes))
					 {
						 TranslaterS(this->textBox1->Text);
						 if ((this->radioButton1->Checked != R[0]) & (this->radioButton1->Checked == true))
						 {
							 R[RButton] = false;
							 WND_W = 100;
							 WND_H = 100;
							 RButton = 0;
							 R[RButton] = true;
						 }
						 if ((this->radioButton2->Checked != R[1]) & (this->radioButton2->Checked == true))
						 {
							 R[RButton] = false;
							 WND_W = 150;
							 WND_H = 150;
							 RButton = 1;
							 R[RButton] = true;
						 }
						 if ((this->radioButton3->Checked != R[2]) & (this->radioButton3->Checked == true))
						 {
							 R[RButton] = false;
							 WND_W = 200;
							 WND_H = 200;
							 RButton = 2;
							 R[RButton] = true;
						 }
						 if ((this->radioButton4->Checked != R[3]) & (this->radioButton4->Checked == true))
						 {
							 R[RButton] = false;
							 WND_W = 250;
							 WND_H = 250;
							 RButton = 3;
							 R[RButton] = true;
						 }
						 if ((this->radioButton5->Checked != R[4]) & (this->radioButton5->Checked == true)) 
						 {
							 R[RButton] = false;
							 WND_W = 300;
							 WND_H = 300;
							 RButton = 4;
							 R[RButton] = true;
						 }
						 Reset();
					 }
					 else goto label;
				 }

				 AutorunChecked = this->checkBox3->Checked;
				 TrayChecked = this->checkBox2->Checked;
				 TaskbarChecked = this->checkBox1->Checked;

				 if (AutorunChecked) AutorunADD(); else AutorunDEL();
				 if (TrayChecked) AddToTray(); else DelFromTray();
				 if (TaskbarChecked) ShowIconT(); else HideIconT();

				 WindowVisIndex = this->hScrollBar1->Value;

				 this->Hide();
label:			 SetS = true;
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {

				 WindowVisibility(WindowVisIndex*255/100);
				 this->hScrollBar1->Value = WindowVisIndex;
				 this->label5->Text = WindowVisIndex.ToString() + "%";
				 this->Hide();
				 SetS = true;
			 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->folderBrowserDialog1->ShowDialog();
			 this->textBox1->Text = this->folderBrowserDialog1->SelectedPath;
		 }
private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (!Compare(this->textBox1->Text)) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void checkBox3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (this->checkBox3->Checked != AutorunChecked) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void checkBox2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (this->checkBox2->Checked!=TrayChecked) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (this->checkBox1->Checked!=TaskbarChecked) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void Settings_Shown(System::Object^  sender, System::EventArgs^  e) {
			 this->button1->Enabled = false;
		 }
private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (this->radioButton1->Checked != R[0]) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (this->radioButton2->Checked != R[1]) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void radioButton3_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (this->radioButton3->Checked != R[2]) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void radioButton4_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (this->radioButton4->Checked != R[3]) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void radioButton5_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (this->radioButton5->Checked != R[4]) this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
private: System::Void hScrollBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 int i = this->hScrollBar1->Value;
			 this->label5->Text = i.ToString()+"%";
			 WindowVisibility(255*i/100);
			 if (i!=WindowVisIndex)this->button1->Enabled = true;
			 else this->button1->Enabled = false;
		 }
};
}