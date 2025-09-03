#include "editor.h"

#include <string>
#include <tinyxml2.h>
using namespace tinyxml2;

void get_element_attr(XMLElement* elem, layout_t* layout){
   for (const XMLAttribute* attr = elem->FirstAttribute(); attr != nullptr; attr=attr->Next()){
      printf("<%s:%s>\n", attr->Name(), attr->Value());
   }
}

void parse_xml(std::string& filename){
   layout_t layout;
   XMLDocument doc;
   XMLError res = doc.LoadFile(filename.c_str());
   if (res != XML_SUCCESS){
      utils::log_info(ERROR, "error in loading xml file");
   }

   XMLElement* root = doc.RootElement();
   std::string layout_name = root->Name();
   printf("layout name: %s\n", layout_name.c_str());
   get_element_attr(root, &layout);
}

void serialize_xml(const std::vector<element_t> data, std::string& filename, layout_type current_layout){
   XMLDocument doc;

   XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\"");
   doc.InsertFirstChild(decl);
   XMLElement* root;
   switch(current_layout){
      case layout_type::linear_layout:
         {
            root = doc.NewElement("LinearLayout");
         }
      break;
      case layout_type::constraint_layout: 
      {
            root = doc.NewElement("ConstraintLayout");
      }
      break;
      case layout_type::frame_layout:
      {
            root = doc.NewElement("FrameLayout");
      }
      break;
      case layout_type::relative_layout:
      {
            root = doc.NewElement("RelativeLayout");
      }
      break;
                                       
   }

   root->SetAttribute("xmlns:android", "http://schemas.android.com/apk/res/android");
   root->SetAttribute("android:layout_width", "match_parent");
   root->SetAttribute("android:layout_height", "match_parent");
   root->SetAttribute("android:gravity", "center");
   root->SetAttribute("android:orientation", "vertical");
   root->SetAttribute("android:background", utils::get_hex_color(State::get_instance()->background_color).c_str());

   
   doc.InsertEndChild(root);
   for (const auto& i: data){
      XMLElement* el = doc.NewElement(i.type.c_str());

      std::string id = "@+id/" + i.id;
      

      std::string pos_x = std::to_string(i.pos.x) + "dp";
      std::string pos_y = std::to_string(i.pos.y) + "dp";

      el->SetAttribute("android:id",   id.c_str());
      el->SetAttribute("android:text", i.text.c_str());
      el->SetAttribute("android:layout_width", "wrap_content");
      el->SetAttribute("android:layout_height", "wrap_content");
      
      if (i.type == "EditText" && i.input_type != "") {
         el->SetAttribute("android:inputType", i.input_type.c_str());
         el->SetAttribute("android:hint", i.hint.c_str());
      }

      root->InsertEndChild(el);

   }
   doc.SaveFile(filename.c_str());
}

