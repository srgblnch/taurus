#ifndef _EXTENSIONFILE_H_
#define _EXTENSIONFILE_H_

#include <ExternalFile.h>

namespace Pool_ns
{

class ExtensionFile: public ExternalFile
{

public:
	ExtensionFile(string &,PoolClass *,Pool *);
	virtual ~ExtensionFile();

protected:
	virtual void load_code();
};

} // namespace

#endif
