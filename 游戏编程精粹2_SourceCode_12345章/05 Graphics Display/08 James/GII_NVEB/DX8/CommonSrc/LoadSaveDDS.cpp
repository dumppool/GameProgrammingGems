/* Copyright (C) Greg James, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Greg James, 2001"
 */
#include "cdds.h"

/////////////////////////////////////////////////////////////////////////////
// CDDS construction/destruction


CDDS::CDDS()
{
    m_ptexOrig = NULL;
    m_ptexNew = NULL;

    m_dwWidth = 0;
    m_dwHeight = 0;
    m_dwDepth = 0;
    m_numMips = 0;

    m_dwCubeMapFlags = 0;
    m_bAllocatedInternally = false;


    //m_bTitleModsChanged = FALSE;
}


CDDS::~CDDS()
{
   if (m_bAllocatedInternally)
        ReleasePpo(&m_ptexOrig);
    //ReleasePpo(&m_ptexNew);
}


BOOL CDDS::MakeTexture(TextureType iTexType, DWORD dwWidth, DWORD dwHeight, int numMips,
                      D3DFORMAT fmt, int dwDepth)
{
    HRESULT hr;



    m_dwWidth = dwWidth;
    m_dwHeight  = dwHeight;
    m_numMips = numMips;

    switch(iTexType)
    {
    case TEXTURE2D:
        LPDIRECT3DTEXTURE8 pmiptex;
        hr = m_pd3ddev->CreateTexture(m_dwWidth, m_dwHeight, m_numMips, 
            0, fmt, D3DPOOL_MANAGED, &pmiptex);
        if (FAILED(hr))
        {
            AfxMessageBox(ID_ERROR_CANTCREATETEXTURE);
            return FALSE;
        }
        m_bAllocatedInternally = true;
        m_ptexOrig = pmiptex;
        break;
    
    case CUBE_TEXTURE:
        // Cube Map
        LPDIRECT3DCUBETEXTURE8 pcubetex;
        m_dwCubeMapFlags = DDS_CUBEMAP_ALLFACES;
        hr = m_pd3ddev->CreateCubeTexture(m_dwWidth, m_numMips, 
            0, fmt, D3DPOOL_MANAGED, &pcubetex);
        if (FAILED(hr))
        {
            AfxMessageBox(ID_ERROR_CANTCREATETEXTURE);
            return FALSE;
        }
        m_bAllocatedInternally = true;
        m_ptexOrig = pcubetex;
        break;

    case VOLUME_TEXTURE:
        LPDIRECT3DVOLUMETEXTURE8 pvoltex;
        m_dwDepth = dwDepth;
        hr = m_pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, m_dwDepth, m_numMips, 
            0, fmt, D3DPOOL_SYSTEMMEM, &pvoltex);
        if (FAILED(hr))
        {
            AfxMessageBox(ID_ERROR_CANTCREATETEXTURE);
            return FALSE;
        }
        m_bAllocatedInternally = true;
        m_ptexOrig = pvoltex;
        break;
    }

    return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CDDS serialization

/*void CDDS::Serialize(CArchive& ar)
{
    HRESULT hr;
    
    if (ar.IsStoring())
    {
        LPDIRECT3DBASETEXTURE8 ptex;
        ptex = (m_ptexNew == NULL ? m_ptexOrig : m_ptexNew);
        SaveDDS(ptex, ar);
    }
    else
    {
        CFile* pFile = ar.GetFile();
        CSTR str = pFile->GetFilePath();
        TCHAR* pszExt = strrchr(str, '.');
        if (lstrcmpi(pszExt, ".bmp") == 0 || lstrcmpi(pszExt, ".tga") == 0)
        {
            if (FAILED(hr = LoadBmp(str)))
                AfxThrowArchiveException(CArchiveException::generic);
        }
        else if (lstrcmpi(pszExt, ".dds") == 0)
        {
            ReleasePpo(&m_ptexOrig);
            ReleasePpo(&m_ptexNew);
            if (FAILED(hr = LoadDDS(&m_ptexOrig, ar)))
                AfxThrowArchiveException(CArchiveException::badIndex); // invalid file format
        }
        else
        {
            AfxThrowArchiveException(CArchiveException::badIndex); // invalid file format
        }
    }
} */


HRESULT CDDS::SaveTexture(LPDIRECT3DTEXTURE8 ptex, const char * filename)
{
    //HRESULT hr;
    D3DSURFACE_DESC sd;
    ptex->GetLevelDesc(0, &sd);
    
    m_dwWidth = sd.Width;
    m_dwHeight  = sd.Height;

    m_numMips = ptex->GetLevelCount();

    m_ptexOrig = ptex;

    return SaveDDS(ptex, filename);

}
HRESULT CDDS::SaveCubeMap(LPDIRECT3DCUBETEXTURE8 pcubetex, const char * filename)
{
    //HRESULT hr;
    D3DSURFACE_DESC sd;
    pcubetex->GetLevelDesc(0, &sd);
    
    m_dwWidth = sd.Width;
    m_dwHeight  = sd.Height;

    m_numMips = pcubetex->GetLevelCount();


    m_ptexOrig = pcubetex;
    
    return SaveDDS(pcubetex, filename);
}

HRESULT CDDS::SaveVolumeTexture(LPDIRECT3DVOLUMETEXTURE8 pvoltex, const char * filename)
{
    //HRESULT hr;
    D3DVOLUME_DESC vd;
    pvoltex->GetLevelDesc(0, &vd);

    m_dwWidth = vd.Width;
    m_dwHeight  = vd.Height;
    m_dwDepth = vd.Depth;

    m_numMips = pvoltex->GetLevelCount();

    m_ptexOrig = pvoltex;


    return SaveDDS(pvoltex, filename);
    
}

HRESULT CDDS::SaveDDS(LPDIRECT3DBASETEXTURE8 ptex, const char * filename)
{
    CArchive  ar;

    HRESULT hr;
    DDS_HEADER ddsh;
    DWORD dwMagic;
    D3DFORMAT fmt;
    DWORD dwSize;
    DWORD dwPitch = 0;
    D3DLOCKED_RECT lr;
    D3DLOCKED_BOX lb;
    LPDIRECT3DTEXTURE8 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE8 pcubetex = NULL;
    LPDIRECT3DVOLUMETEXTURE8 pvoltex = NULL;

    ar.Open(filename, "wb");

    if (IsVolumeMap())
        pvoltex = (LPDIRECT3DVOLUMETEXTURE8)ptex;
    else if (!IsCubeMap())
        pmiptex = (LPDIRECT3DTEXTURE8)ptex;
    else
        pcubetex = (LPDIRECT3DCUBETEXTURE8)ptex;

    dwMagic = MAKEFOURCC('D','D','S',' ');
    ar.Write(&dwMagic, sizeof(dwMagic));

    // Fill in the DDS header structure
    ZeroMemory(&ddsh, sizeof(ddsh));
    ddsh.dwSize = sizeof(ddsh);
    ddsh.dwHeaderFlags = DDS_HEADER_FLAGS_TEXTURE;
    ddsh.dwWidth = m_dwWidth;
    ddsh.dwHeight = m_dwHeight;
    ddsh.dwSurfaceFlags = DDS_SURFACE_FLAGS_TEXTURE;
    if (m_numMips > 1)
    {
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_MIPMAP;
        ddsh.dwSurfaceFlags |= DDS_SURFACE_FLAGS_MIPMAP;
        ddsh.dwMipMapCount = m_numMips;
    }
    if (pvoltex != NULL)
    {
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_VOLUME;
        // Note: "dwCubemapFlags" is mostly for cubemap flags,
        // but in this case is also used to indicate a volume texture
        ddsh.dwCubemapFlags |= DDS_FLAGS_VOLUME;
        ddsh.dwDepth = m_dwDepth;
    }
    
    // In DX7, you could (optionally) use and save a subset of 
    // cubemap faces.  In DX8, you must use and save all faces of a cubemap.
    if (IsCubeMap())
    {
        ddsh.dwSurfaceFlags |= DDS_SURFACE_FLAGS_CUBEMAP;
        ddsh.dwCubemapFlags = DDS_CUBEMAP_ALLFACES;
    }

    // Determine dwPitch and dwSize for the texture
    if (pvoltex != NULL)
    {
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        fmt = vd.Format;
        dwSize = vd.Size;
        if (SUCCEEDED(pvoltex->LockBox(0, &lb, NULL, D3DLOCK_READONLY)))
        {
            dwPitch = lb.RowPitch;
            pvoltex->UnlockBox(0);
        }
    }
    else if (pmiptex != NULL)
    {
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
        dwSize = sd.Size;
        if (SUCCEEDED(pmiptex->LockRect(0, &lr, NULL, D3DLOCK_READONLY)))
        {
            dwPitch = lr.Pitch;
            pmiptex->UnlockRect(0);
        }
    }
    else
    {
        D3DSURFACE_DESC sd;
        pcubetex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
        dwSize = sd.Size;
        if (SUCCEEDED(pcubetex->LockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0, 
            &lr, NULL, D3DLOCK_READONLY)))
        {
            dwPitch = lr.Pitch;
            pcubetex->UnlockRect(D3DCUBEMAP_FACE_POSITIVE_X, 0);
        }
    }

    // Note that although this code stores the pitch or linear size
    // of the surface in the DDS file (for consistency with DX7), it
    // is better to use the pitch or linear size of the surface
    // created when loading the DDS file into a texture than to use
    // the file values.  The pitch of a created surface, especially, 
    // can be different from the pitch stored in the DDS file.
    switch (fmt)
    {
    case D3DFMT_DXT1:
        ddsh.ddspf = DDSPF_DXT1;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.dwPitchOrLinearSize = dwSize;
        break;
    case D3DFMT_DXT2:
        ddsh.ddspf = DDSPF_DXT2;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.dwPitchOrLinearSize = dwSize;
        break;
    case D3DFMT_DXT3:
        ddsh.ddspf = DDSPF_DXT3;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.dwPitchOrLinearSize = dwSize;
        break;
    case D3DFMT_DXT4:
        ddsh.ddspf = DDSPF_DXT4;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.dwPitchOrLinearSize = dwSize;
        break;
    case D3DFMT_DXT5:
        ddsh.ddspf = DDSPF_DXT5;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_LINEARSIZE;
        ddsh.dwPitchOrLinearSize = dwSize;
        break;
    case D3DFMT_A8R8G8B8:
        ddsh.ddspf = DDSPF_A8R8G8B8;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_PITCH;
        ddsh.dwPitchOrLinearSize = dwPitch;
        break;
    case D3DFMT_A1R5G5B5:
        ddsh.ddspf = DDSPF_A1R5G5B5;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_PITCH;
        ddsh.dwPitchOrLinearSize = dwPitch;
        break;
    case D3DFMT_A4R4G4B4:
        ddsh.ddspf = DDSPF_A4R4G4B4;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_PITCH;
        ddsh.dwPitchOrLinearSize = dwPitch;
        break;
    case D3DFMT_R8G8B8:
        ddsh.ddspf = DDSPF_R8G8B8;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_PITCH;
        ddsh.dwPitchOrLinearSize = dwPitch;
        break;
    case D3DFMT_R5G6B5:
        ddsh.ddspf = DDSPF_R5G6B5;
        ddsh.dwHeaderFlags |= DDS_HEADER_FLAGS_PITCH;
        ddsh.dwPitchOrLinearSize = dwPitch;
        break;
    default:
        return E_FAIL;
    }

    ar.Write(&ddsh, sizeof(ddsh));

    if (pvoltex != NULL)
    {
        if (FAILED(hr = SaveAllVolumeSurfaces(pvoltex, ar)))
            return hr;
    }
    else if (pmiptex != NULL)
    {
        if (FAILED(hr = SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_FORCE_DWORD, ar)))
            return hr;
    }
    else
    {
        if (FAILED(hr = SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_POSITIVE_X, ar)))
            return hr;
        if (FAILED(hr = SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_NEGATIVE_X, ar)))
            return hr;
        if (FAILED(hr = SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_POSITIVE_Y, ar)))
            return hr;
        if (FAILED(hr = SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_NEGATIVE_Y, ar)))
            return hr;
        if (FAILED(hr = SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_POSITIVE_Z, ar)))
            return hr;
        if (FAILED(hr = SaveAllMipSurfaces(ptex, D3DCUBEMAP_FACE_NEGATIVE_Z, ar)))
            return hr;
    }

    ar.Close();
    return S_OK;
}


HRESULT CDDS::SaveAllMipSurfaces(LPDIRECT3DBASETEXTURE8 ptex, D3DCUBEMAP_FACES FaceType, CArchive& ar)
{
    HRESULT hr;
    LPDIRECT3DSURFACE8 psurf;
    D3DSURFACE_DESC sd;
    UINT iLevel;
    D3DLOCKED_RECT lr;
    LPDIRECT3DTEXTURE8 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE8 pcubetex = NULL;

    if (FaceType == D3DCUBEMAP_FACE_FORCE_DWORD)
        pmiptex = (LPDIRECT3DTEXTURE8)ptex;
    else
        pcubetex = (LPDIRECT3DCUBETEXTURE8)ptex;

    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        if (pmiptex != NULL)
            hr = pmiptex->GetSurfaceLevel(iLevel, &psurf);
        else
            hr = pcubetex->GetCubeMapSurface(FaceType, iLevel, &psurf);
        if (FAILED(hr))
            return hr;
        psurf->GetDesc(&sd);
        if (pmiptex != NULL)
            hr = pmiptex->LockRect(iLevel, &lr, NULL, 0);
        else
            hr = pcubetex->LockRect(FaceType, iLevel, &lr, NULL, 0);
        if (FAILED(hr))
            return hr;
        if (sd.Format == D3DFMT_DXT1 ||
            sd.Format == D3DFMT_DXT2 ||
            sd.Format == D3DFMT_DXT3 ||
            sd.Format == D3DFMT_DXT4 ||
            sd.Format == D3DFMT_DXT5)
        {
            ar.Write(lr.pBits, sd.Size);
        }
        else
        {
            DWORD yp;
            BYTE* pbDest = (BYTE*)lr.pBits;
            LONG dataBytesPerRow = 0;
            if (sd.Format == D3DFMT_A8R8G8B8)
                dataBytesPerRow = 4 * sd.Width;
            else if (sd.Format == D3DFMT_R8G8B8)
                dataBytesPerRow = 3 * sd.Width;
            else
                dataBytesPerRow = 2 * sd.Width;
            for (yp = 0; yp < sd.Height; yp++)
            {
                ar.Write(pbDest, dataBytesPerRow);
                pbDest += lr.Pitch;
            }
        }
        if (pmiptex != NULL)
            hr = pmiptex->UnlockRect(iLevel);
        else
            hr = pcubetex->UnlockRect(FaceType, iLevel);
    }

    return S_OK;
}


HRESULT CDDS::SaveAllVolumeSurfaces(LPDIRECT3DVOLUMETEXTURE8 pvoltex, CArchive& ar)
{
    HRESULT hr;
    D3DVOLUME_DESC vd;
    D3DBOX box;
    D3DLOCKED_BOX lb;
    UINT iLevel;
    UINT numBytesPerRow;
    BYTE* pbSlice;
    BYTE* pbRow;
    UINT zp;
    UINT yp;

    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        pvoltex->GetLevelDesc(iLevel, &vd);
        box.Left = 0;
        box.Right = vd.Width;
        box.Top = 0;
        box.Bottom = vd.Height;
        box.Front = 0;
        box.Back = vd.Depth;
        hr = pvoltex->LockBox(iLevel, &lb, &box, 0);
        if (FAILED(hr))
            return hr;
        switch(vd.Format)
        {
        case D3DFMT_A8R8G8B8:
            numBytesPerRow = 4 * vd.Width;
            break;
        case D3DFMT_R8G8B8:
            numBytesPerRow = 3 * vd.Width;
            break;
        case D3DFMT_A1R5G5B5:
        case D3DFMT_A4R4G4B4:
        case D3DFMT_R5G6B5:
            numBytesPerRow = 2 * vd.Width;
            break;
        default:
            return E_FAIL;
        }
        pbSlice = (BYTE*)lb.pBits;
        for (zp = 0; zp < vd.Depth; zp++)
        {
            pbRow = pbSlice;
            for (yp = 0; yp < vd.Height; yp++)
            {
                ar.Write(pbRow, numBytesPerRow);
                pbRow += lb.RowPitch;
            }
            pbSlice += lb.SlicePitch;
        }

        pvoltex->UnlockBox(iLevel);
    }

    return S_OK;
}

// loads a texture and returns a pointer in pptex

HRESULT CDDS::LoadDDS(LPDIRECT3DBASETEXTURE8* pptex, const char * filename)
{
    CArchive ar;
    ar.Open(filename, "rb");

    HRESULT hr;
    DWORD dwMagic;
    DDS_HEADER ddsh;
    LPDIRECT3DTEXTURE8 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE8 pcubetex = NULL;
    LPDIRECT3DVOLUMETEXTURE8 pvoltex = NULL;

    *pptex = NULL;
    
    ar.Read(&dwMagic, sizeof(dwMagic));
    if (dwMagic != MAKEFOURCC('D','D','S',' '))
        return E_FAIL;
    ar.Read(&ddsh, sizeof(ddsh));
    if (ddsh.dwSize != sizeof(ddsh))
        return E_FAIL;
    m_dwWidth = ddsh.dwWidth;
    m_dwHeight = ddsh.dwHeight;
    m_numMips = ddsh.dwMipMapCount;
    if (m_numMips == 0)
        m_numMips = 1;

    m_dwCubeMapFlags = (ddsh.dwCubemapFlags & DDS_CUBEMAP_ALLFACES);
    if (ddsh.dwHeaderFlags & DDS_HEADER_FLAGS_VOLUME)
        m_dwDepth = ddsh.dwDepth;
    else
        m_dwDepth = 0;

    D3DFORMAT fmt;
    if (!IsVolumeMap() && ddsh.ddspf.dwFourCC == D3DFMT_DXT1)
        fmt = D3DFMT_DXT1;
    else if (!IsVolumeMap() && ddsh.ddspf.dwFourCC == D3DFMT_DXT2)
        fmt = D3DFMT_DXT2;
    else if (!IsVolumeMap() && ddsh.ddspf.dwFourCC == D3DFMT_DXT3)
        fmt = D3DFMT_DXT3;
    else if (!IsVolumeMap() && ddsh.ddspf.dwFourCC == D3DFMT_DXT4)
        fmt = D3DFMT_DXT4;
    else if (!IsVolumeMap() && ddsh.ddspf.dwFourCC == D3DFMT_DXT5)
        fmt = D3DFMT_DXT5;
    else if (ddsh.ddspf.dwFlags == DDS_RGBA && ddsh.ddspf.dwRGBBitCount == 32 && ddsh.ddspf.dwABitMask == 0xff000000)
        fmt = D3DFMT_A8R8G8B8;
    else if (ddsh.ddspf.dwFlags == DDS_RGB  && ddsh.ddspf.dwRGBBitCount == 24)
        fmt = D3DFMT_R8G8B8;
    else if (ddsh.ddspf.dwFlags == DDS_RGB  && ddsh.ddspf.dwRGBBitCount == 16 && ddsh.ddspf.dwGBitMask == 0x000007e0)
        fmt = D3DFMT_R5G6B5;
    else if (ddsh.ddspf.dwFlags == DDS_RGBA && ddsh.ddspf.dwRGBBitCount == 16 && ddsh.ddspf.dwABitMask == 0x00008000)
        fmt = D3DFMT_A1R5G5B5;
    else if (ddsh.ddspf.dwFlags == DDS_RGBA && ddsh.ddspf.dwRGBBitCount == 16 && ddsh.ddspf.dwABitMask == 0x0000f000)
        fmt = D3DFMT_A4R4G4B4;
    else
        return E_FAIL; // unknown fmt

    if (IsVolumeMap())
    {
        if (FAILED(hr = m_pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, m_dwDepth, m_numMips,
            0, fmt, D3DPOOL_SYSTEMMEM, &pvoltex)))
        {
            return hr;
        }

        if (FAILED(hr = LoadAllVolumeSurfaces(pvoltex, ar)))
            return hr;

        *pptex = pvoltex;
    }
    else if (IsCubeMap())
    {
        if (FAILED(hr = m_pd3ddev->CreateCubeTexture(m_dwWidth, m_numMips, 
             0, fmt, D3DPOOL_MANAGED, &pcubetex)))
        {
            return hr;
        }

        // Cubemaps created with the DX7 version of DxTex may skip some
        // cube faces.  ddsh.dwCubeMapFlags indicates which faces are
        // present.  If you only care about loading post-DX7 cubemaps, you
        // don't have to check these flags -- just load each face in sequence.

        // REVIEW: zero out surfaces of missing faces?

        if (m_dwCubeMapFlags & DDS_CUBEMAP_POSITIVEX)
        {
            if (FAILED(hr = LoadAllMipSurfaces(pcubetex, D3DCUBEMAP_FACE_POSITIVE_X, ar)))
                return hr;
        }

        if (m_dwCubeMapFlags & DDS_CUBEMAP_NEGATIVEX)
        {
            if (FAILED(hr = LoadAllMipSurfaces(pcubetex, D3DCUBEMAP_FACE_NEGATIVE_X, ar)))
                return hr;
        }

        if (m_dwCubeMapFlags & DDS_CUBEMAP_POSITIVEY)
        {
            if (FAILED(hr = LoadAllMipSurfaces(pcubetex, D3DCUBEMAP_FACE_POSITIVE_Y, ar)))
                return hr;
        }

        if (m_dwCubeMapFlags & DDS_CUBEMAP_NEGATIVEY)
        {
            if (FAILED(hr = LoadAllMipSurfaces(pcubetex, D3DCUBEMAP_FACE_NEGATIVE_Y, ar)))
                return hr;
        }

        if (m_dwCubeMapFlags & DDS_CUBEMAP_POSITIVEZ)
        {
            if (FAILED(hr = LoadAllMipSurfaces(pcubetex, D3DCUBEMAP_FACE_POSITIVE_Z, ar)))
                return hr;
        }

        if (m_dwCubeMapFlags & DDS_CUBEMAP_NEGATIVEZ)
        {
            if (FAILED(hr = LoadAllMipSurfaces(pcubetex, D3DCUBEMAP_FACE_NEGATIVE_Z, ar)))
                return hr;
        }

        *pptex = pcubetex;
    }
    else
    {
        if (FAILED(hr = m_pd3ddev->CreateTexture(m_dwWidth, m_dwHeight, m_numMips, 
             0, fmt, D3DPOOL_MANAGED, &pmiptex)))
        {
            return hr;
        }

        if (FAILED(hr = LoadAllMipSurfaces(pmiptex, D3DCUBEMAP_FACE_FORCE_DWORD, ar)))
            return hr;

        *pptex = pmiptex;
    }

    ar.Close();
    return S_OK;
}


HRESULT CDDS::LoadAllMipSurfaces(LPDIRECT3DBASETEXTURE8 ptex, D3DCUBEMAP_FACES FaceType, CArchive& ar)
{
    HRESULT hr;
    LPDIRECT3DSURFACE8 psurf;
    D3DSURFACE_DESC sd;
    UINT iLevel;
    D3DLOCKED_RECT lr;
    LPDIRECT3DTEXTURE8 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE8 pcubetex = NULL;
    DWORD dwBytesPerRow;

    if (FaceType == D3DCUBEMAP_FACE_FORCE_DWORD)
        pmiptex = (LPDIRECT3DTEXTURE8)ptex;
    else
        pcubetex = (LPDIRECT3DCUBETEXTURE8)ptex;

    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        if (pmiptex != NULL)
            hr = pmiptex->GetSurfaceLevel(iLevel, &psurf);
        else
            hr = pcubetex->GetCubeMapSurface(FaceType, iLevel, &psurf);
        if (FAILED(hr))
            return hr;
        psurf->GetDesc(&sd);
        switch (sd.Format)
        {
        case D3DFMT_DXT1:
        case D3DFMT_DXT2:
        case D3DFMT_DXT3:
        case D3DFMT_DXT4:
        case D3DFMT_DXT5:
            dwBytesPerRow = 0; // magic value indicates texture's memory is contiguous
            break;
        case D3DFMT_A8R8G8B8:
            dwBytesPerRow = 4 * sd.Width;
            break;
        case D3DFMT_R8G8B8:
            dwBytesPerRow = 3 * sd.Width;
            break;
        case D3DFMT_A1R5G5B5:
        case D3DFMT_A4R4G4B4:
        case D3DFMT_R5G6B5:
            dwBytesPerRow = 2 * sd.Width;
            break;
        default:
            return E_FAIL;
        }
            
        if (pmiptex != NULL)
            hr = pmiptex->LockRect(iLevel, &lr, NULL, 0);
        else
            hr = pcubetex->LockRect(FaceType, iLevel, &lr, NULL, 0);
        if (FAILED(hr))
            return hr;
        if (dwBytesPerRow == 0)
        {
            ar.Read(lr.pBits, sd.Size);
        }
        else
        {
            DWORD yp;
            BYTE* pbDest = (BYTE*)lr.pBits;
            for (yp = 0; yp < sd.Height; yp++)
            {
                ar.Read(pbDest, dwBytesPerRow);
                pbDest += lr.Pitch;
            }
        }

        if (pmiptex != NULL)
            hr = pmiptex->UnlockRect(iLevel);
        else
            hr = pcubetex->UnlockRect(FaceType, iLevel);
        ReleasePpo(&psurf);
    }

    return S_OK;
}


HRESULT CDDS::LoadAllVolumeSurfaces(LPDIRECT3DVOLUMETEXTURE8 pvoltex, CArchive& ar)
{
    HRESULT hr;
    D3DVOLUME_DESC vd;
    D3DBOX box;
    D3DLOCKED_BOX lb;
    UINT iLevel;
    UINT numBytesPerRow;
    BYTE* pbSlice;
    BYTE* pbRow;
    UINT zp;
    UINT yp;

    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        pvoltex->GetLevelDesc(iLevel, &vd);
        box.Left = 0;
        box.Right = vd.Width;
        box.Top = 0;
        box.Bottom = vd.Height;
        box.Front = 0;
        box.Back = vd.Depth;
        hr = pvoltex->LockBox(iLevel, &lb, &box, 0);
        if (FAILED(hr))
            return hr;
        switch(vd.Format)
        {
        case D3DFMT_A8R8G8B8:
            numBytesPerRow = 4 * vd.Width;
            break;
        default:
            return E_FAIL;
        }
        pbSlice = (BYTE*)lb.pBits;
        for (zp = 0; zp < vd.Depth; zp++)
        {
            pbRow = pbSlice;
            for (yp = 0; yp < vd.Height; yp++)
            {
                ar.Read(pbRow, numBytesPerRow);
                pbRow += lb.RowPitch;
            }
            pbSlice += lb.SlicePitch;
        }

        pvoltex->UnlockBox(iLevel);
    }

    return S_OK;
}

bool FileExists(std::string & name)
{
    FILE * fp;
    
    fp = fopen(name.c_str(), "rb");
    if (fp)
    {
        fclose(fp);
        return true;
    }
    else
        return false;
}

/////////////////////////////////////////////////////////////////////////////
// CDDS commands

HRESULT CDDS::LoadBmp(CSTR& strPath)
{
    HRESULT hr;

    LPDIRECT3DTEXTURE8 ptex;

    hr = D3DXCreateTextureFromFileEx(m_pd3ddev, strPath.c_str(), D3DX_DEFAULT, 
        D3DX_DEFAULT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, 
        D3DX_FILTER_POINT, D3DX_FILTER_POINT, 0, NULL, NULL, &ptex);
    if (FAILED(hr))
        return hr;

    // Look for "foo_a.bmp" for alpha channel
    int i = strPath.rfind('.');
    //strPath = strPath.Left(i) + "_a.bmp";

    CSTR strPath2;
    strPath2 = strPath.substr(0, i)  + "_a.bmp";

    if (FileExists(strPath2))
    {
        LPDIRECT3DSURFACE8 psurf;

        hr = ptex->GetSurfaceLevel(0, &psurf);
        if (FAILED(hr))
            return hr;

        hr = LoadAlphaIntoSurface(strPath2, psurf);
        ReleasePpo(&psurf);
        if (FAILED(hr))
            return hr;
    }
    
    // Ensure that source image dimensions are power of 2
    D3DSURFACE_DESC sd;
    ptex->GetLevelDesc(0, &sd);
    m_dwWidth = sd.Width;
    m_dwHeight = sd.Height;

    LONG lwTempW;
    LONG lwTempH;
    lwTempW = sd.Width;
    lwTempH = sd.Height;
    while ((lwTempW & 1) == 0)
        lwTempW = lwTempW >> 1;
    while ((lwTempH & 1) == 0)
        lwTempH = lwTempH >> 1;
    if (lwTempW != 1 || lwTempH != 1)
    {
        AfxMessageBox(ID_ERROR_NOTPOW2);
        ReleasePpo(&ptex);
        return E_FAIL;
    }

    m_numMips = 1;

    ReleasePpo(&m_ptexOrig);
    m_ptexOrig = ptex;

    m_strPathName.empty();
    
    return S_OK;
}


HRESULT CDDS::LoadAlphaBmp(CSTR& strPath)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE8 pmiptex;
    LPDIRECT3DSURFACE8 psurf;

    if (IsCubeMap())
        return E_FAIL;

    pmiptex = (LPDIRECT3DTEXTURE8)m_ptexOrig;
    hr = pmiptex->GetSurfaceLevel(0, &psurf);
    if (FAILED(hr))
        return hr;

    hr = LoadAlphaIntoSurface(strPath, psurf);
    ReleasePpo(&psurf);
    if (FAILED(hr))
        return hr;
    
    return S_OK;
}


HRESULT CDDS::Compress(D3DFORMAT fmtTo, BOOL bSwitchView)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE8 pmiptex;
    LPDIRECT3DCUBETEXTURE8 pcubetex;
    LPDIRECT3DVOLUMETEXTURE8 pvoltex;
    D3DFORMAT fmtFrom;
    LPDIRECT3DTEXTURE8 pmiptexNew;
    LPDIRECT3DCUBETEXTURE8 pcubetexNew;
    LPDIRECT3DVOLUMETEXTURE8 pvoltexNew;

    if (IsVolumeMap())
    {
        if (fmtTo == D3DFMT_DXT1 ||
            fmtTo == D3DFMT_DXT2 ||
            fmtTo == D3DFMT_DXT3 ||
            fmtTo == D3DFMT_DXT4 ||
            fmtTo == D3DFMT_DXT5)
        {
            return E_FAIL;
        }
        pvoltex = (LPDIRECT3DVOLUMETEXTURE8)m_ptexOrig;
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        fmtFrom = vd.Format;
    }
    else if (IsCubeMap())
    {
        pcubetex = (LPDIRECT3DCUBETEXTURE8)m_ptexOrig;
        D3DSURFACE_DESC sd;
        pcubetex->GetLevelDesc(0, &sd);
        fmtFrom = sd.Format;
    }
    else
    {
        pmiptex = (LPDIRECT3DTEXTURE8)m_ptexOrig;
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        fmtFrom = sd.Format;
    }

    if (fmtFrom == D3DFMT_DXT2 || fmtFrom == D3DFMT_DXT4)
    {
        if (fmtTo == D3DFMT_DXT1)
        {
            AfxMessageBox(ID_ERROR_PREMULTTODXT1);
        }
        else if (fmtTo != D3DFMT_DXT2 && fmtTo != D3DFMT_DXT4)
        {
            AfxMessageBox(ID_ERROR_PREMULTALPHA);
            return S_OK;
        }
    }

    if (IsVolumeMap())
    {
        hr = m_pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, m_dwDepth, m_numMips,
            0, fmtTo, D3DPOOL_SYSTEMMEM, &pvoltexNew);
        if (FAILED(hr))
            return hr;
        ReleasePpo(&m_ptexNew);
        m_ptexNew = pvoltexNew;
        if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, m_ptexOrig, m_ptexNew)))
            return hr;
    }
    else if (IsCubeMap())
    {
        hr = m_pd3ddev->CreateCubeTexture(m_dwWidth, m_numMips, 
             0, fmtTo, D3DPOOL_MANAGED, &pcubetexNew);
        if (FAILED(hr))
            return hr;
        ReleasePpo(&m_ptexNew);
        m_ptexNew = pcubetexNew;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_X, m_ptexOrig, m_ptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_X, m_ptexOrig, m_ptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_Y, m_ptexOrig, m_ptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_Y, m_ptexOrig, m_ptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_Z, m_ptexOrig, m_ptexNew)))
            return hr;
        if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_Z, m_ptexOrig, m_ptexNew)))
            return hr;
    }
    else
    {
        hr = m_pd3ddev->CreateTexture(m_dwWidth, m_dwHeight, m_numMips, 
             0, fmtTo, D3DPOOL_MANAGED, &pmiptexNew);
        if (FAILED(hr))
            return hr;
        ReleasePpo(&m_ptexNew);
        m_ptexNew = pmiptexNew;
        if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, m_ptexOrig, m_ptexNew)))
            return hr;
    }

    //SetModifiedFlag();
    //m_bTitleModsChanged = TRUE; // force title bar update
    //if (bSwitchView)
    //    AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_VIEW_COMPRESSED, 0);

    return S_OK;
}


/*void CDDS::OnGenerateMipMaps() 
{
    GenerateMipMaps();
}
  */

void CDDS::GenerateMipMaps() 
{
    LONG lwTempH;
    LONG lwTempW;
    LONG lwPowsW;
    LONG lwPowsH;
    LPDIRECT3DTEXTURE8 pddsNew = NULL;
    D3DFORMAT fmt;
    HRESULT hr;
    //LPDIRECT3DDEVICE8 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE8 pmiptex = NULL;
    LPDIRECT3DCUBETEXTURE8 pcubetex = NULL;
    LPDIRECT3DVOLUMETEXTURE8 pvoltex = NULL;
    LPDIRECT3DTEXTURE8 pmiptexNew = NULL;
    LPDIRECT3DCUBETEXTURE8 pcubetexNew = NULL;
    LPDIRECT3DVOLUMETEXTURE8 pvoltexNew = NULL;
    LPDIRECT3DSURFACE8 psurfSrc;
    LPDIRECT3DSURFACE8 psurfDest;
    LPDIRECT3DVOLUME8 pvolSrc;
    LPDIRECT3DVOLUME8 pvolDest;

    if (IsVolumeMap())
        pvoltex = (LPDIRECT3DVOLUMETEXTURE8)m_ptexOrig;
    else if (IsCubeMap())
        pcubetex = (LPDIRECT3DCUBETEXTURE8)m_ptexOrig;
    else
        pmiptex = (LPDIRECT3DTEXTURE8)m_ptexOrig;

    if (pvoltex != NULL)
    {
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        fmt = vd.Format;
    }
    else if (pcubetex != NULL)
    {
        D3DSURFACE_DESC sd;
        pcubetex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }
    else
    {
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        fmt = sd.Format;
    }

    // Ensure that source image is power of 2
    lwTempW = m_dwWidth;
    lwTempH = m_dwHeight;
    lwPowsW = 1;
    lwPowsH = 1;
    while ((lwTempW & 1) == 0)
    {
        lwPowsW++;
        lwTempW = lwTempW >> 1;
    }
    while ((lwTempH & 1) == 0)
    {
        lwPowsH++;
        lwTempH = lwTempH >> 1;
    }
    if (lwTempW != 1 || lwTempH != 1)
    {
        AfxMessageBox(ID_ERROR_NOTPOW2);
        return;
    }
    m_numMips = lwPowsW > lwPowsH ? lwPowsW : lwPowsH;

    // Create destination mipmap surface - same format as source
    if (pvoltex != NULL)
    {
        if (FAILED(hr = m_pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, m_dwDepth, 
            m_numMips, 0, fmt, D3DPOOL_SYSTEMMEM, &pvoltexNew)))
        {
            goto LFail;
        }
        hr = pvoltex->GetVolumeLevel(0, &pvolSrc);
        hr = pvoltexNew->GetVolumeLevel(0, &pvolDest);
        hr = D3DXLoadVolumeFromVolume(pvolDest, NULL, NULL, pvolSrc, NULL, NULL, 
            D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&pvolSrc);
        ReleasePpo(&pvolDest);
        hr = D3DXFilterVolumeTexture(pvoltexNew, NULL, 0, D3DX_FILTER_TRIANGLE);
    }
    else if (pmiptex != NULL)
    {
        if (FAILED(hr = m_pd3ddev->CreateTexture(m_dwWidth, m_dwHeight, m_numMips, 
             0, fmt, D3DPOOL_MANAGED, &pmiptexNew)))
        {
            goto LFail;
        }
        hr = pmiptex->GetSurfaceLevel(0, &psurfSrc);
        hr = pmiptexNew->GetSurfaceLevel(0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = D3DXFilterTexture(pmiptexNew, NULL, 0, D3DX_FILTER_TRIANGLE);
    }
    else
    {
        if (FAILED(hr = m_pd3ddev->CreateCubeTexture(m_dwWidth, m_numMips, 
             0, fmt, D3DPOOL_MANAGED, &pcubetexNew)))
        {
            goto LFail;
        }
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_X, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_X, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Y, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Y, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_POSITIVE_Z, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = pcubetex->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &psurfSrc);
        hr = pcubetexNew->GetCubeMapSurface(D3DCUBEMAP_FACE_NEGATIVE_Z, 0, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, psurfSrc, NULL, NULL, 
            D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
        hr = D3DXFilterCubeTexture(pcubetexNew, NULL, 0, D3DX_FILTER_TRIANGLE);
    }

    ReleasePpo(&m_ptexOrig);
    if (pvoltexNew != NULL)
        m_ptexOrig = pvoltexNew;
    else if (pcubetexNew != NULL)
        m_ptexOrig = pcubetexNew;
    else
        m_ptexOrig = pmiptexNew;

    if (m_ptexNew != NULL)
    {
        // Rather than filtering down the (probably-compressed) m_ptexNew 
        // top level, compress each mip level from the (probably-uncompressed)
        // m_ptexOrig levels.
        if (pvoltexNew != NULL)
        {
            D3DVOLUME_DESC vd;
            ((LPDIRECT3DVOLUMETEXTURE8)m_ptexNew)->GetLevelDesc(0, &vd);
            fmt = vd.Format;
        }
        else if (pcubetexNew != NULL)
        {
            D3DSURFACE_DESC sd;
            ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetLevelDesc(0, &sd);
            fmt = sd.Format;
        }
        else
        {
            D3DSURFACE_DESC sd;
            ((LPDIRECT3DCUBETEXTURE8)m_ptexNew)->GetLevelDesc(0, &sd);
            fmt = sd.Format;
        }
        Compress(fmt, FALSE);
    }

    //m_bTitleModsChanged = TRUE; // Generate title bar update
    //UpdateAllViews(NULL, 1); // tell CView to pick up new surface pointers
    //SetModifiedFlag();
    return;

LFail:
    ReleasePpo(&pddsNew);
}


void CDDS::SetPathName(LPCTSTR lpszPathName, BOOL bAddToMRU) 
{
    //CDocument::SetPathName(lpszPathName, bAddToMRU);

    TCHAR* pszLeaf = strrchr(lpszPathName, '\\') + 1;
    TCHAR* pszExtension = strrchr(lpszPathName, '.');
    if (lstrcmpi(pszExtension, ".dds") != 0)
    {
        lstrcpy(pszExtension, "");
        //SetModifiedFlag(TRUE);
        //SetTitle(pszLeaf);
        m_strPathName.empty();
    }
}

DWORD CDDS::NumMips(VOID)
{
    return m_numMips;
}

/*
void CDDS::OnFileOpenAlpha() 
{
    HRESULT hr;
    CSTR fileName;
    LPDIRECT3DTEXTURE8 pmiptex;

    if (IsCubeMap() || IsVolumeMap())
        return;
    pmiptex = (LPDIRECT3DTEXTURE8)m_ptexOrig;

    // Premultiplied-alpha files don't support this feature:
    D3DSURFACE_DESC sd;
    pmiptex->GetLevelDesc(0, &sd);
    if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
    {
        AfxMessageBox(ID_ERROR_PREMULTALPHA);
        return;
    }

    if (!PromptForBmp(&fileName))
        return;

    LPDIRECT3DSURFACE8 psurf;
    if (FAILED(hr = pmiptex->GetSurfaceLevel(0, &psurf)))
        return;

    if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurf)))
        return;
    if (m_numMips > 1)
        OnGenerateMipMaps();
    else if (m_ptexNew != NULL)
    {
        ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetLevelDesc(0, &sd);
        Compress(sd.Format, FALSE);
    }
    UpdateAllViews(NULL, 1);
}
*/


HRESULT CDDS::LoadAlphaIntoSurface(CSTR& strPath, LPDIRECT3DSURFACE8 psurf)
{
    HRESULT hr;
    D3DSURFACE_DESC sd;
    //LPDIRECT3DDEVICE8 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE8 ptexAlpha;
    LPDIRECT3DSURFACE8 psurfAlpha;
    LPDIRECT3DSURFACE8 psurfTarget;

    psurf->GetDesc(&sd);

    // Load the alpha BMP into psurfAlpha, a new A8R8G8B8 surface
    hr = D3DXCreateTextureFromFileEx(m_pd3ddev, strPath.c_str(), sd.Width, sd.Height, 1, 0, 
        D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE, 
        D3DX_FILTER_TRIANGLE, 0, NULL, NULL, &ptexAlpha);
    hr = ptexAlpha->GetSurfaceLevel(0, &psurfAlpha);

    // Copy the target surface into an A8R8G8B8 surface
    hr = m_pd3ddev->CreateImageSurface(sd.Width, sd.Height, D3DFMT_A8R8G8B8, &psurfTarget);
    hr = D3DXLoadSurfaceFromSurface(psurfTarget, NULL, NULL, psurf, NULL, NULL, 
        D3DX_FILTER_TRIANGLE, 0);

    // Fill in the alpha channels of psurfTarget based on the blue channel of psurfAlpha
    D3DLOCKED_RECT lrSrc;
    D3DLOCKED_RECT lrDest;

    hr = psurfAlpha->LockRect(&lrSrc, NULL, D3DLOCK_READONLY);
    hr = psurfTarget->LockRect(&lrDest, NULL, 0);

    DWORD xp;
    DWORD yp;
    DWORD* pdwRowSrc = (DWORD*)lrSrc.pBits;
    DWORD* pdwRowDest = (DWORD*)lrDest.pBits;
    DWORD* pdwSrc;
    DWORD* pdwDest;
    DWORD dwAlpha;
    LONG dataBytesPerRow = 4 * sd.Width;

    for (yp = 0; yp < sd.Height; yp++)
    {
        pdwSrc = pdwRowSrc;
        pdwDest = pdwRowDest;
        for (xp = 0; xp < sd.Width; xp++)
        {
            dwAlpha = *pdwSrc << 24;
            *pdwDest &= 0x00ffffff;
            *pdwDest |= dwAlpha;

            pdwSrc++;
            pdwDest++;
        }
        pdwRowSrc += lrSrc.Pitch / 4;
        pdwRowDest += lrDest.Pitch / 4;
    }

    psurfAlpha->UnlockRect();
    psurfTarget->UnlockRect();

    // Copy psurfTarget back into real surface
    hr = D3DXLoadSurfaceFromSurface(psurf, NULL, NULL, psurfTarget, NULL, NULL, 
        D3DX_FILTER_TRIANGLE, 0);
    
    // Release allocated interfaces
    ReleasePpo(&psurfTarget);
    ReleasePpo(&psurfAlpha);
    ReleasePpo(&ptexAlpha);

    return S_OK;
}

/*
BOOL CDDS::PromptForBmp(CSTR* pstrPath)
{
    CFileDialog dlgFile(TRUE);

    CSTR title;
    VERIFY(title.LoadString(AFX_IDS_OPENFILE));

    CSTR strFilter;
    CSTR strDefault;

    strFilter += "Bitmap Files (*.bmp, *.tga)";
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.bmp;*.tga");
    strFilter += (TCHAR)'\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;

    // append the "*.*" all files filter
    CSTR allFilter;
    VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
    strFilter += allFilter;
    strFilter += (TCHAR)'\0';   // next string please
    strFilter += _T("*.*");
    strFilter += (TCHAR)'\0';   // last string
    dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrTitle = title;
    dlgFile.m_ofn.lpstrFile = pstrPath->GetBuffer(_MAX_PATH);

    INT_PTR nResult = dlgFile.DoModal();
    pstrPath->ReleaseBuffer();
    if (nResult != IDOK)
        return FALSE;
    return TRUE;
}        */


void CDDS::OpenSubsurface(CSTR fileName, D3DCUBEMAP_FACES FaceType, LONG lwMip, LONG lwSlice)
{
    HRESULT hr;
    //CSTR fileName;
    //LPDIRECT3DDEVICE8 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE8 ptex = NULL;
    LPDIRECT3DSURFACE8 psurfOrig = NULL;
    LPDIRECT3DSURFACE8 psurfNew = NULL;

    //if (!PromptForBmp(&fileName))
    //    return;

    if (IsVolumeMap())
    {
        hr = D3DXCreateTextureFromFile(m_pd3ddev, fileName.c_str(), &ptex);
        hr = ptex->GetSurfaceLevel(0, &psurfOrig);
    }
    else if (IsCubeMap())
    {
        hr = ((LPDIRECT3DCUBETEXTURE8)m_ptexOrig)->GetCubeMapSurface(FaceType, lwMip, &psurfOrig);
        if (m_ptexNew != NULL)
            hr = ((LPDIRECT3DCUBETEXTURE8)m_ptexNew)->GetCubeMapSurface(FaceType, lwMip, &psurfNew);
        hr = D3DXLoadSurfaceFromFile(psurfOrig, NULL, NULL, fileName.c_str(), NULL, D3DX_FILTER_TRIANGLE, 0, NULL);
    }
    else
    {
        hr = ((LPDIRECT3DTEXTURE8)m_ptexOrig)->GetSurfaceLevel(lwMip, &psurfOrig);
        if (m_ptexNew != NULL)
            hr = ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetSurfaceLevel(lwMip, &psurfNew);
        hr = D3DXLoadSurfaceFromFile(psurfOrig, NULL, NULL, fileName.c_str(), NULL, D3DX_FILTER_TRIANGLE, 0, NULL);
    }

    // Look for "foo_a.bmp" for alpha channel
    int i = fileName.rfind('.');
    //fileName = fileName.Left(i) + "_a.bmp";

    CSTR strPath2;
    strPath2 = fileName.substr(0, i)  + "_a.bmp";

    
    //CFileStatus status;
    if (FileExists(strPath2))
    {
        if (FAILED(hr = LoadAlphaIntoSurface(strPath2, psurfOrig)))
            return;
    }

    if (IsVolumeMap())
    {
        LPDIRECT3DVOLUME8 pvol;
        hr = ((LPDIRECT3DVOLUMETEXTURE8)m_ptexOrig)->GetVolumeLevel(lwMip, &pvol);
        hr = LoadVolumeSliceFromSurface(pvol, lwSlice, psurfOrig);
        ReleasePpo(&pvol);
        if (m_ptexNew)
        {
            hr = ((LPDIRECT3DVOLUMETEXTURE8)m_ptexNew)->GetVolumeLevel(lwMip, &pvol);
            hr = LoadVolumeSliceFromSurface(pvol, lwSlice, psurfOrig);
            ReleasePpo(&pvol);
        }
    }
    else if (psurfNew != NULL)
    {
        hr = D3DXLoadSurfaceFromSurface(psurfNew, NULL, NULL, psurfOrig, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
    }

    ReleasePpo(&psurfOrig);
    ReleasePpo(&psurfNew);
    ReleasePpo(&ptex);

    //SetModifiedFlag(TRUE);
    //UpdateAllViews(NULL, 1);
}


void CDDS::OpenAlphaSubsurface(CSTR fileName, D3DCUBEMAP_FACES FaceType, LONG lwMip, LONG lwSlice)
{
    HRESULT hr;
    //CSTR fileName;
    //LPDIRECT3DDEVICE8 pd3ddev = PDxtexApp()->Pd3ddev();
    LPDIRECT3DTEXTURE8 ptexOrig = NULL;
    LPDIRECT3DTEXTURE8 ptexNew = NULL;
    LPDIRECT3DSURFACE8 psurfOrig = NULL;
    LPDIRECT3DSURFACE8 psurfNew = NULL;
    LPDIRECT3DVOLUME8 pvolOrig = NULL;
    LPDIRECT3DVOLUME8 pvolNew = NULL;
    D3DSURFACE_DESC sd;
    DWORD dwWidth = m_dwWidth;
    DWORD dwHeight = m_dwHeight;

    if (IsVolumeMap())
    {
        for (int i = 0; i < lwMip; i++)
        {
            dwWidth /= 2;
            dwHeight /= 2;
        }
        hr = m_pd3ddev->CreateTexture(dwWidth, dwHeight, 1, 
             0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ptexOrig);
        hr = ptexOrig->GetSurfaceLevel(0, &psurfOrig);
        hr = ((LPDIRECT3DVOLUMETEXTURE8)m_ptexOrig)->GetVolumeLevel(lwMip, &pvolOrig);
        hr = LoadSurfaceFromVolumeSlice(pvolOrig, lwSlice, psurfOrig);
        if (m_ptexNew != NULL)
        {
            hr = m_pd3ddev->CreateTexture(dwWidth, dwHeight, 1, 
                 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &ptexNew);
            hr = ptexNew->GetSurfaceLevel(0, &psurfOrig);
            hr = ((LPDIRECT3DVOLUMETEXTURE8)m_ptexOrig)->GetVolumeLevel(lwMip, &pvolNew);
            hr = LoadSurfaceFromVolumeSlice(pvolNew, lwSlice, psurfOrig);
        }
    }
    else if (IsCubeMap())
    {
        hr = ((LPDIRECT3DCUBETEXTURE8)m_ptexOrig)->GetCubeMapSurface(FaceType, lwMip, &psurfOrig);
        ((LPDIRECT3DCUBETEXTURE8)m_ptexOrig)->GetLevelDesc(lwMip, &sd);
        if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
        {
            AfxMessageBox(ID_ERROR_PREMULTALPHA);
            return;
        }
        if (m_ptexNew != NULL)
        {
            hr = ((LPDIRECT3DCUBETEXTURE8)m_ptexNew)->GetCubeMapSurface(FaceType, lwMip, &psurfNew);
            ((LPDIRECT3DCUBETEXTURE8)m_ptexNew)->GetLevelDesc(lwMip, &sd);
            if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
            {
                AfxMessageBox(ID_ERROR_PREMULTALPHA);
                return;
            }
        }
    }
    else
    {
        hr = ((LPDIRECT3DTEXTURE8)m_ptexOrig)->GetSurfaceLevel(lwMip, &psurfOrig);
        ((LPDIRECT3DTEXTURE8)m_ptexOrig)->GetLevelDesc(lwMip, &sd);
        if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
        {
            AfxMessageBox(ID_ERROR_PREMULTALPHA);
            return;
        }
        if (m_ptexNew != NULL)
        {
            hr = ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetSurfaceLevel(lwMip, &psurfNew);
            ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetLevelDesc(lwMip, &sd);
            if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
            {
                AfxMessageBox(ID_ERROR_PREMULTALPHA);
                return;
            }
        }
    }

    //if (!PromptForBmp(&fileName))
    //    return;

    if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfOrig)))
        return;

    if (psurfNew != NULL)
    {
        if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfNew)))
            return;
    }

    if (pvolOrig != NULL)
    {
        hr = LoadVolumeSliceFromSurface(pvolOrig, lwSlice, psurfOrig);
    }
    if (pvolNew != NULL)
    {
        hr = LoadVolumeSliceFromSurface(pvolNew, lwSlice, psurfNew);
    }

    ReleasePpo(&psurfOrig);
    ReleasePpo(&psurfNew);
    ReleasePpo(&ptexOrig);
    ReleasePpo(&ptexNew);
    ReleasePpo(&pvolOrig);
    ReleasePpo(&pvolNew);

    //SetModifiedFlag(TRUE);
    //UpdateAllViews(NULL, 1);
}


/*void CDDS::OnFormatChangeCubeMapFaces() 
{
    HRESULT hr;
    LPDIRECT3DDEVICE8 pd3ddev = PDxtexApp()->Pd3ddev();
    D3DSURFACE_DESC sd;
    LPDIRECT3DCUBETEXTURE8 ptexCube;
    DWORD iLevel;
    LPDIRECT3DSURFACE8 psurfSrc;
    LPDIRECT3DSURFACE8 psurfDest;

    CCubeMapDlg cubeMapDlg;
    if (IDCANCEL == cubeMapDlg.DoModal())
        return;
    
    // Change m_ptexOrig into a cubemap
    ((LPDIRECT3DTEXTURE8)m_ptexOrig)->GetLevelDesc(0, &sd);
    hr = D3DXCreateCubeTexture(pd3ddev, m_dwWidth, m_numMips, 0, sd.Format, D3DPOOL_MANAGED, &ptexCube);
    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        hr = ((LPDIRECT3DTEXTURE8)m_ptexOrig)->GetSurfaceLevel(iLevel, &psurfSrc);
        hr = ptexCube->GetCubeMapSurface((D3DCUBEMAP_FACES)cubeMapDlg.m_iFace, iLevel, &psurfDest);
        hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
            psurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&psurfDest);
    }
    ReleasePpo(&m_ptexOrig);
    m_ptexOrig = ptexCube;

    // Change m_ptexNew into a cubemap too
    if (m_ptexNew != NULL)
    {
        ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetLevelDesc(0, &sd);
        hr = D3DXCreateCubeTexture(pd3ddev, m_dwWidth, m_numMips, 0, sd.Format, D3DPOOL_MANAGED, &ptexCube);
        for (iLevel = 0; iLevel < m_numMips; iLevel++)
        {
            hr = ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetSurfaceLevel(iLevel, &psurfSrc);
            hr = ptexCube->GetCubeMapSurface((D3DCUBEMAP_FACES)cubeMapDlg.m_iFace, iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
            ReleasePpo(&psurfSrc);
            ReleasePpo(&psurfDest);
        }
        ReleasePpo(&m_ptexNew);
        m_ptexNew = ptexCube;
    }
    m_dwCubeMapFlags = DDS_CUBEMAP_ALLFACES;
    SetModifiedFlag();
    UpdateAllViews(NULL, 1); // tell CView to pick up new surface pointers
}
       


void CDDS::OnFormatMakeIntoVolumeMap() 
{
    HRESULT hr;
    //LPDIRECT3DDEVICE8 pd3ddev = PDxtexApp()->Pd3ddev();
    D3DSURFACE_DESC sd;
    LPDIRECT3DVOLUMETEXTURE8 ptexVolume;
    DWORD iLevel;
    LPDIRECT3DSURFACE8 psurfSrc;
    LPDIRECT3DVOLUME8 pvolumeDest;
    UINT numLayers;

    CVolumeMapDlg volumeMapDlg;
    if (IDCANCEL == volumeMapDlg.DoModal())
        return;

    numLayers = (1 << volumeMapDlg.m_powLayers) * 2;

    // Change m_ptexOrig into a volumemap
    ((LPDIRECT3DTEXTURE8)m_ptexOrig)->GetLevelDesc(0, &sd);
    hr = pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, numLayers,
        m_numMips, 0, sd.Format, D3DPOOL_SYSTEMMEM, &ptexVolume);
    if (FAILED(hr))
        return;
    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        hr = ((LPDIRECT3DTEXTURE8)m_ptexOrig)->GetSurfaceLevel(iLevel, &psurfSrc);
        hr = ptexVolume->GetVolumeLevel(iLevel, &pvolumeDest);
        hr = LoadVolumeSliceFromSurface(pvolumeDest, 0, psurfSrc);
        ReleasePpo(&psurfSrc);
        ReleasePpo(&pvolumeDest);
    }
    ReleasePpo(&m_ptexOrig);
    m_ptexOrig = ptexVolume;

    // Change m_ptexNew into a volumemap too
    if (m_ptexNew != NULL)
    {
        ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetLevelDesc(0, &sd);
        hr = pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, numLayers,
            m_numMips, 0, sd.Format, D3DPOOL_SYSTEMMEM, &ptexVolume);
        if (FAILED(hr))
            return;
        for (iLevel = 0; iLevel < m_numMips; iLevel++)
        {
            hr = ((LPDIRECT3DTEXTURE8)m_ptexNew)->GetSurfaceLevel(iLevel, &psurfSrc);
            hr = ptexVolume->GetVolumeLevel(iLevel, &pvolumeDest);
            hr = LoadVolumeSliceFromSurface(pvolumeDest, 0, psurfSrc);
            ReleasePpo(&psurfSrc);
            ReleasePpo(&pvolumeDest);
        }
        ReleasePpo(&m_ptexNew);
        m_ptexNew = ptexVolume;
    }
    m_dwDepth = numLayers;
    SetModifiedFlag();
    UpdateAllViews(NULL, 1); // tell CView to pick up new surface pointers
}
       */

HRESULT CDDS::LoadVolumeSliceFromSurface(LPDIRECT3DVOLUME8 pVolume, UINT iSlice, LPDIRECT3DSURFACE8 psurf)
{
    HRESULT hr;
    D3DSURFACE_DESC sd;
    D3DVOLUME_DESC vd;
    D3DLOCKED_RECT lr;
    D3DBOX boxSrc;
    D3DBOX boxDest;

    psurf->GetDesc(&sd);
    pVolume->GetDesc(&vd);

    boxSrc.Left = 0;
    boxSrc.Right = sd.Width;
    boxSrc.Top = 0;
    boxSrc.Bottom = sd.Height;
    boxSrc.Front = 0;
    boxSrc.Back = 1;

    boxDest.Left = 0;
    boxDest.Right = vd.Width;
    boxDest.Top = 0;
    boxDest.Bottom = vd.Height;
    boxDest.Front = iSlice;
    boxDest.Back = iSlice + 1;

    hr = psurf->LockRect(&lr, NULL, 0);
    if (FAILED(hr))
        return hr;

    hr = D3DXLoadVolumeFromMemory(pVolume, NULL, &boxDest, lr.pBits, sd.Format, lr.Pitch, 
        0, NULL, &boxSrc, D3DX_FILTER_TRIANGLE, 0);

    psurf->UnlockRect();

    return hr;
}


HRESULT CDDS::LoadSurfaceFromVolumeSlice(LPDIRECT3DVOLUME8 pVolume, UINT iSlice, LPDIRECT3DSURFACE8 psurf)
{
    HRESULT hr;
    D3DVOLUME_DESC vd;
    D3DLOCKED_BOX lb;
    D3DBOX box;
    RECT rc;

    pVolume->GetDesc(&vd);

    box.Left = 0;
    box.Right = vd.Width;
    box.Top = 0;
    box.Bottom = vd.Height;
    box.Front = iSlice;
    box.Back = iSlice + 1;

    rc.left = 0;
    rc.right = vd.Width;
    rc.top = 0;
    rc.bottom = vd.Height;

    hr = pVolume->LockBox(&lb, &box, 0);
    if (FAILED(hr))
        return hr;

    hr = D3DXLoadSurfaceFromMemory(psurf, NULL, NULL, lb.pBits, vd.Format, lb.RowPitch, 
        NULL, &rc, D3DX_FILTER_TRIANGLE, 0);

    pVolume->UnlockBox();

    return hr;
}


HRESULT CDDS::BltAllLevels(D3DCUBEMAP_FACES FaceType, 
    LPDIRECT3DBASETEXTURE8 ptexSrc, LPDIRECT3DBASETEXTURE8 ptexDest)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE8 pmiptexSrc;
    LPDIRECT3DTEXTURE8 pmiptexDest;
    LPDIRECT3DCUBETEXTURE8 pcubetexSrc;
    LPDIRECT3DCUBETEXTURE8 pcubetexDest;
    LPDIRECT3DVOLUMETEXTURE8 pvoltexSrc;
    LPDIRECT3DVOLUMETEXTURE8 pvoltexDest;
    LPDIRECT3DSURFACE8 psurfSrc;
    LPDIRECT3DSURFACE8 psurfDest;
    LPDIRECT3DVOLUME8 pvolSrc;
    LPDIRECT3DVOLUME8 pvolDest;
    DWORD iLevel;

    if (IsVolumeMap())
    {
        pvoltexSrc = (LPDIRECT3DVOLUMETEXTURE8)ptexSrc;
        pvoltexDest = (LPDIRECT3DVOLUMETEXTURE8)ptexDest;
    }
    else if (IsCubeMap())
    {
        pcubetexSrc = (LPDIRECT3DCUBETEXTURE8)ptexSrc;
        pcubetexDest = (LPDIRECT3DCUBETEXTURE8)ptexDest;
    }
    else
    {
        pmiptexSrc = (LPDIRECT3DTEXTURE8)ptexSrc;
        pmiptexDest = (LPDIRECT3DTEXTURE8)ptexDest;
    }

    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        if (IsVolumeMap())
        {
            hr = pvoltexSrc->GetVolumeLevel(iLevel, &pvolSrc);
            hr = pvoltexDest->GetVolumeLevel(iLevel, &pvolDest);
            hr = D3DXLoadVolumeFromVolume(pvolDest, NULL, NULL, 
                pvolSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
        }
        if (IsCubeMap())
        {
            hr = pcubetexSrc->GetCubeMapSurface(FaceType, iLevel, &psurfSrc);
            hr = pcubetexDest->GetCubeMapSurface(FaceType, iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
        }
        else
        {
            hr = pmiptexSrc->GetSurfaceLevel(iLevel, &psurfSrc);
            hr = pmiptexDest->GetSurfaceLevel(iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
        }
    }

    return S_OK;
}


void CDDS::OpenCubeFace(CSTR fileName, D3DCUBEMAP_FACES FaceType)
{
    HRESULT hr;
    
    LPDIRECT3DSURFACE8 psurfOrig = NULL;
    LPDIRECT3DSURFACE8 psurfNew = NULL;

    if (!IsCubeMap())
        return;

    hr = ((LPDIRECT3DCUBETEXTURE8)m_ptexOrig)->GetCubeMapSurface(FaceType, 0, &psurfOrig);
    if (m_ptexNew != NULL)
        hr = ((LPDIRECT3DCUBETEXTURE8)m_ptexNew)->GetCubeMapSurface(FaceType, 0, &psurfNew);

    //if (!PromptForBmp(&fileName))
    //    return;

    hr = D3DXLoadSurfaceFromFile(psurfOrig, NULL, NULL, fileName.c_str(), NULL, D3DX_FILTER_TRIANGLE, 0, NULL);

    // Look for "foo_a.bmp" for alpha channel
    int i = fileName.rfind('.');

    //fileName = fileName.Left(i) + "_a.bmp";
    CSTR fileName2 = fileName.substr(0, i) + "_a.bmp";

    //CFileStatus status;
    if (FileExists(fileName2))
    {
        if (FAILED(hr = LoadAlphaIntoSurface(fileName2, psurfOrig)))
            return;
    }

    if (m_numMips > 1)
    {
        hr = D3DXFilterCubeTexture((LPDIRECT3DCUBETEXTURE8)m_ptexOrig, NULL, 0, D3DX_FILTER_TRIANGLE);
    }


    if (psurfNew != NULL)
    {
        hr = D3DXLoadSurfaceFromSurface(psurfNew, NULL, NULL, psurfOrig, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);

        if (m_numMips > 1)
        {
            hr = D3DXFilterCubeTexture((LPDIRECT3DCUBETEXTURE8)m_ptexNew, NULL, 0, D3DX_FILTER_TRIANGLE);
        }
    }

    ReleasePpo(&psurfOrig);
    ReleasePpo(&psurfNew);

    //SetModifiedFlag(TRUE);
    //UpdateAllViews(NULL, 1);
}



void CDDS::OpenAlphaCubeFace(CSTR fileName, D3DCUBEMAP_FACES FaceType)
{
    HRESULT hr;
    
    LPDIRECT3DSURFACE8 psurfOrig = NULL;
    LPDIRECT3DSURFACE8 psurfNew = NULL;
    D3DSURFACE_DESC sd;

    if (!IsCubeMap())
        return;

    hr = ((LPDIRECT3DCUBETEXTURE8)m_ptexOrig)->GetCubeMapSurface(FaceType, 0, &psurfOrig);
    ((LPDIRECT3DCUBETEXTURE8)m_ptexOrig)->GetLevelDesc(0, &sd);
    if (sd.Format == D3DFMT_DXT2 || sd.Format == D3DFMT_DXT4)
    {
        AfxMessageBox(ID_ERROR_PREMULTALPHA);
        return;
    }
    if (m_ptexNew != NULL)
    {
        hr = ((LPDIRECT3DCUBETEXTURE8)m_ptexNew)->GetCubeMapSurface(FaceType, 0, &psurfNew);
    }

    //if (!PromptForBmp(&fileName))
    //    return;

    if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfOrig)))
        return;

    if (psurfNew != NULL)
    {
        if (FAILED(hr = LoadAlphaIntoSurface(fileName, psurfNew)))
            return;
    }

    if (m_numMips > 1)
    {
        hr = D3DXFilterCubeTexture((LPDIRECT3DCUBETEXTURE8)m_ptexOrig, NULL, 0, D3DX_FILTER_TRIANGLE);
    }


    if (psurfNew != NULL)
    {
        hr = D3DXLoadSurfaceFromSurface(psurfNew, NULL, NULL, psurfOrig, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);

        if (m_numMips > 1)
        {
            hr = D3DXFilterCubeTexture((LPDIRECT3DCUBETEXTURE8)m_ptexNew, NULL, 0, D3DX_FILTER_TRIANGLE);
        }
    }

    ReleasePpo(&psurfOrig);
    ReleasePpo(&psurfNew);

    //SetModifiedFlag(TRUE);
    //UpdateAllViews(NULL, 1);
}


DWORD CDDS::DwDepthAt(LONG lwMip)
{
    DWORD dwDepth = m_dwDepth;
    while (lwMip > 0 && dwDepth > 1)
    {
        dwDepth /= 2;
        lwMip--;
    }
    return dwDepth;
}


