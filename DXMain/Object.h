#pragma once

class CObject {
public:
	//----------------------------object-----------------------------
	bool Begin();
	virtual bool End();
	//----------------------------object-----------------------------

	tag GetTag() { return m_tag; }
	string GetName() { return m_name; }
	void SetTag(tag t) { m_tag = t; }
	void SetName(string name) { m_name = name; }

private:
	string m_name;
	tag m_tag{ tag::TAG_DEFAULT };

public:
	CObject(string name, tag t = tag::TAG_DEFAULT);
	virtual ~CObject();
};