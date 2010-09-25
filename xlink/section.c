/*  Copyright 2008 Carsten S�rensen

    This file is part of ASMotor.

    ASMotor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ASMotor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ASMotor.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "xlink.h"

SSection* pSections=NULL;

SSection* sect_CreateNew(void)
{
	SSection* *ppsect;

	ppsect=&pSections;

	while(*ppsect)
	{
		ppsect=&(*ppsect)->pNext;
	}

	if((*ppsect=(SSection* )malloc(sizeof(SSection)))!=NULL)
	{
		(*ppsect)->pNext=NULL;
		(*ppsect)->Used=FALSE;
		(*ppsect)->Assigned=FALSE;
		(*ppsect)->pPatches=NULL;
		return* ppsect;
	}

	Error("Out of memory");
	return NULL;
}

static	void	resolve_symbol(SSection* sect, SSymbol* sym)
{
	switch(sym->Type)
	{
		case	SYM_LOCALEXPORT:
		case	SYM_EXPORT:
		case	SYM_LOCAL:
		{
			sym->Resolved=TRUE;
			sym->Value+=sect->Org;
			sym->pSection=sect;
			break;
		}
		case	SYM_IMPORT:
		{
			SSection* filesect;

			filesect=pSections;

			while(filesect)
			{
				if(filesect->Used)
				{
					ULONG	i;
					SSymbol* filesym;

					filesym=filesect->pSymbols;

					for(i=0; i<filesect->TotalSymbols; i+=1)
					{
						if( (filesym->Type==SYM_EXPORT)
						&&	strcmp(filesym->Name,sym->Name)==0 )
						{
							if(!filesym->Resolved)
							{
								resolve_symbol(filesect, filesym);
							}

							sym->Resolved=TRUE;
							sym->Value=filesym->Value;
							sym->pSection=filesect;

							return;
						}
						filesym+=1;
					}
				}
				filesect=filesect->pNext;
			}

			Error("unresolved symbol \"%s\"", sym->Name);
			break;
		}
		case	SYM_LOCALIMPORT:
		{
			SSection* filesect;

			filesect=pSections;

			while(filesect)
			{
				if( filesect->Used 
				&&	filesect->FileID==sect->FileID )
				{
					ULONG	i;
					SSymbol* filesym;

					filesym=filesect->pSymbols;

					for(i=0; i<filesect->TotalSymbols; i+=1)
					{
						if( ((filesym->Type==SYM_LOCALEXPORT)||(filesym->Type==SYM_EXPORT))
						&&	strcmp(filesym->Name,sym->Name)==0 )
						{
							if(!filesym->Resolved)
							{
								resolve_symbol(filesect, filesym);
							}

							sym->Resolved=TRUE;
							sym->Value=filesym->Value;
							sym->pSection=filesect;

							return;
						}
						filesym+=1;
					}
				}
				filesect=filesect->pNext;
			}

			Error("unresolved symbol \"%s\"", sym->Name);
			break;
		}
		default:
		{
			Error("unhandled symbol type");
			break;
		}
	}
}

SLONG	sect_GetSymbolValue(SSection* sect, SLONG symbolid)
{
	SSymbol* sym;

	sym=&sect->pSymbols[symbolid];

	if(!sym->Resolved)
	{
		resolve_symbol(sect, sym);
	}

	return sym->Value;
}

SLONG	sect_GetSymbolBank(SSection* sect, SLONG symbolid)
{
	SSymbol* sym;

	sym=&sect->pSymbols[symbolid];

	if(!sym->Resolved)
	{
		resolve_symbol(sect, sym);
	}

	return sym->pSection->Bank;
}