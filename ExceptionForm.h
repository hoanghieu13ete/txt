#pragma once

namespace anprs {



	/// <summary>
	/// Summary for ExceptionForm
	/// </summary>
	public ref class ExceptionForm : public System::Windows::Forms::Form
	{
	public:
		ExceptionForm(void)
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
		~ExceptionForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btn_Ok;
	protected:
	private: System::Windows::Forms::Button^  btn_Cancel;
	private: System::Windows::Forms::TextBox^  txt_InputText;
	public:	 System::String^ textInput;
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
			this->btn_Ok = (gcnew System::Windows::Forms::Button());
			this->btn_Cancel = (gcnew System::Windows::Forms::Button());
			this->txt_InputText = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// btn_Ok
			// 
			this->btn_Ok->Location = System::Drawing::Point(35, 79);
			this->btn_Ok->Name = L"btn_Ok";
			this->btn_Ok->Size = System::Drawing::Size(75, 23);
			this->btn_Ok->TabIndex = 0;
			this->btn_Ok->Text = L"Ok";
			this->btn_Ok->UseVisualStyleBackColor = true;
			this->btn_Ok->Click += gcnew System::EventHandler(this, &ExceptionForm::btn_Ok_Click);
			// 
			// btn_Cancel
			// 
			this->btn_Cancel->Location = System::Drawing::Point(155, 79);
			this->btn_Cancel->Name = L"btn_Cancel";
			this->btn_Cancel->Size = System::Drawing::Size(75, 23);
			this->btn_Cancel->TabIndex = 1;
			this->btn_Cancel->Text = L"Cancel";
			this->btn_Cancel->UseVisualStyleBackColor = true;
			this->btn_Cancel->Click += gcnew System::EventHandler(this, &ExceptionForm::btn_Cancel_Click);
			// 
			// txt_InputText
			// 
			this->txt_InputText->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->txt_InputText->Location = System::Drawing::Point(35, 27);
			this->txt_InputText->Name = L"txt_InputText";
			this->txt_InputText->Size = System::Drawing::Size(195, 29);
			this->txt_InputText->TabIndex = 2;
			// 
			// ExceptionForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(272, 122);
			this->Controls->Add(this->txt_InputText);
			this->Controls->Add(this->btn_Cancel);
			this->Controls->Add(this->btn_Ok);
			this->Name = L"ExceptionForm";
			this->Text = L"ExceptionForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btn_Cancel_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void btn_Ok_Click(System::Object^  sender, System::EventArgs^  e);
	};
}
