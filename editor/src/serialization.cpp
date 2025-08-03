#include "editor.h"

#include <string>
#include <tinyxml2.h>
using namespace tinyxml2;

void parse_xml(std::string& filename){
   XMLDocument doc;
   XMLError res = doc.LoadFile(filename.c_str());
   if (res != XML_SUCCESS){
      utils::log_info(ERROR, "error in loading xml file");
   }

   XMLElement* root = doc.RootElement();

   //TODO... extract resources
   
}

void serialize_xml(const std::vector<element_t> data, std::string& filename){
   XMLDocument doc;

   XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\"");
   doc.InsertFirstChild(decl);

   XMLElement* root = doc.NewElement("LinearLayout");
   root->SetAttribute("xmlns:android", "http://schemas.android.com/apk/res/android");
   root->SetAttribute("android:layout_width", "match_parent");
   root->SetAttribute("android:layout_height", "match_parent");
   root->SetAttribute("android:gravity", "center");
   root->SetAttribute("android:orientation", "vertical");
   
   doc.InsertEndChild(root);
   for (const auto& i: data){
      XMLElement* el = doc.NewElement(i.type.c_str());

      std::string id = "@+id/" + i.id;
      

      std::string pos_x = std::to_string(i.pos.x) + "dp";
      std::string pos_y = std::to_string(i.pos.y) + "dp";

      el->SetAttribute("android:id", id.c_str());
      el->SetAttribute("android:text", i.text.c_str());
      el->SetAttribute("android:layout_width", "wrap_content");
      el->SetAttribute("android:layout_height", "wrap_content");

      //FIXME
      // el->SetAttribute("android:layout_marginLeft", pos_x.c_str());
      // el->SetAttribute("android:layout_marginTop", pos_y.c_str());
      root->InsertEndChild(el);

   }
   doc.SaveFile(filename.c_str());
}
