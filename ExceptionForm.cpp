#include "ExceptionForm.h"
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

inline System::Void anprs::ExceptionForm::btn_Cancel_Click(System::Object ^ sender, System::EventArgs ^ e) {
	this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
	this->Close();
}

inline System::Void anprs::ExceptionForm::btn_Ok_Click(System::Object ^ sender, System::EventArgs ^ e) {
	textInput = txt_InputText->Text;
	this->DialogResult = System::Windows::Forms::DialogResult::OK;
	this->Close();
}
