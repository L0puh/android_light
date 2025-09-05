#include "editor.h"

#include <string>
#include <tinyxml2.h>
using namespace tinyxml2;

void get_element_attr(XMLElement* elem, layout_t *layout) {
   std::map<std::string, std::string> m;
   for (const XMLAttribute* attr = elem->FirstAttribute(); attr != nullptr; attr=attr->Next()){
      m[attr->Name()] = attr->Value();
   
   }
   if (m.find("android:layout_width") != m.end()){
      layout->width = m["android:layout_width"];
   }
   if (m.find("android:layout_height") != m.end()){
      layout->height= m["android:layout_height"];
   }
   if (m.find("android:gravity") != m.end()){
      layout->gravity = m["android:gravity"];

   }
   if (m.find("android:orientation") != m.end()){
      layout->orientation = m["android:orientation"];

   }
   if (m.find("android:background") != m.end()){
      //TODO: convert from hex to vector
   }
}

layout_type get_layout_type_from_name(const std::string& name){
   if (name == "LinearLayout") return layout_type::linear_layout;
   if (name == "ConstraintLayout") return layout_type::constraint_layout;
   if (name == "FrameLayout") return layout_type::frame_layout;
   if (name == "RelativeLayout") return layout_type::relative_layout;
   return layout_type::linear_layout; //FIXME: fix to unknown
}

void parse_xml(std::string& filename, layout_t *parsed_layout, std::vector<element_t> *parsed_elements){
   const std::string and_prefix = "android:", id_prefix = "@+id/";

   XMLDocument doc;
   XMLError res = doc.LoadFile(filename.c_str());
   if (res != XML_SUCCESS){
      utils::log_info(ERROR, "error in loading xml file");
   }

   XMLElement* root = doc.RootElement();
   parsed_layout->type = get_layout_type_from_name(root->Name());
   std::map<std::string, std::string> attributes;
   get_element_attr(root, parsed_layout);

   for (XMLElement* elem = root->FirstChildElement();
               elem != nullptr; elem = elem->NextSiblingElement()){

      element_t parsed_elem;
      parsed_elem.type = elem->Name();
      if (parsed_elem.type.find(and_prefix) == 0)
         parsed_elem.type = parsed_elem.type.substr(and_prefix.length());

      //parse attributes 
      for (const XMLAttribute* attr = elem->FirstAttribute(); attr != nullptr; attr = attr->Next()){
         std::string attr_name = attr->Name();
         std::string attr_value = attr->Value();
            if (attr_name == "android:id") {
                parsed_elem.id = attr_value;
                if (parsed_elem.id.find(id_prefix) == 0){
                   parsed_elem.id = parsed_elem.id.substr(id_prefix.length());
                }
            } else if (attr_name == "android:text") {
                parsed_elem.text = attr_value;
            } else if (attr_name == "android:inputType") {
                parsed_elem.input_type = attr_value;
            } else if (attr_name == "android:hint") {
                parsed_elem.hint = attr_value;
            }
      }
      parsed_elements->push_back(parsed_elem);
   }
}

void serialize_xml(const std::vector<element_t> data, std::string& filename, layout_t& layout){
   XMLDocument doc;

   XMLDeclaration* decl = doc.NewDeclaration("xml version=\"1.0\" encoding=\"utf-8\"");
   doc.InsertFirstChild(decl);
   XMLElement* root;
   switch(layout.type){
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
   root->SetAttribute("android:gravity",  layout.gravity.c_str());
   root->SetAttribute("android:orientation", layout.orientation.c_str()); 
   root->SetAttribute("android:background", utils::get_hex_color(State::get_instance()->background_color).c_str());

   
   doc.InsertEndChild(root);
   for (const auto& i: data){
      XMLElement* el = doc.NewElement(i.type.c_str());

      std::string id = "@+id/" + i.id;
      

      std::string pos_x = std::to_string(i.pos.x) + "dp";
      std::string pos_y = std::to_string(i.pos.y) + "dp";

      el->SetAttribute("android:id",   id.c_str());
      el->SetAttribute("android:text", i.text.c_str());
      el->SetAttribute("android:layout_width",  "wrap_content");
      el->SetAttribute("android:layout_height", "wrap_content");

      if (layout_type::frame_layout == layout.type){
        el->SetAttribute("android:layout_marginStart", pos_x.c_str());
        el->SetAttribute("android:layout_marginTop", pos_y.c_str());
          
      }
      
      if (i.type == "EditText" && i.input_type != "") {
         el->SetAttribute("android:inputType", i.input_type.c_str());
         el->SetAttribute("android:hint", i.hint.c_str());
      }

      root->InsertEndChild(el);

   }
   doc.SaveFile(filename.c_str());
}

