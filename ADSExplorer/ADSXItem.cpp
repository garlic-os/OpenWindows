#include "stdafx.h"  // MUST be included first

#include "ADSXItem.h"


bool CADSXItem::IsOwn(PCUIDLIST_RELATIVE pidl) {
  return pidl != NULL &&
		 pidl->mkid.cb == sizeof(ITEMIDLIST) + sizeof(CADSXItem) - sizeof(BYTE) &&
		 ILIsChild(pidl) &&
		 CADSXItem::Get((PCUITEMID_CHILD) pidl)->SIGNATURE == 'ADSX';
}


CADSXItem *CADSXItem::Get(PCUITEMID_CHILD pidl) {
	return (CADSXItem *) &pidl->mkid.abID;
}


PITEMID_CHILD CADSXItem::ToPidl() const {
	// The item copy is manually allocated, as opposed to using C++'s `new`,
	// because COM requires memory to be allocated with CoTaskMemAlloc.
	UINT cbSizeItem = sizeof(SHITEMID) - sizeof(BYTE) + sizeof(CADSXItem);
	UINT cbSizeItemList = cbSizeItem + sizeof(SHITEMID);

	// Allocate memory for this SHITEMID plus the final null SHITEMID.
	auto pidlNew = (PITEMID_CHILD) CoTaskMemAlloc(cbSizeItemList);
	if (pidlNew == NULL) return NULL;

	// Put the data object in the PIDL
	new (pidlNew->mkid.abID) CADSXItem();
	(CADSXItem &) pidlNew->mkid.abID = *this;
	pidlNew->mkid.cb = cbSizeItem;

	// A sentinel PIDL at the end of the list as the ITEMIDLIST spec ordains
	PUITEMID_CHILD pidlEnd = (PUITEMID_CHILD) ILNext(pidlNew);
	pidlEnd->mkid.abID[0] = 0;
	pidlEnd->mkid.cb = 0;

	return pidlNew;
}