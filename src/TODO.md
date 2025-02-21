RHI:
- кеширование стейтов при установке
- при бинде индекс буфера ставить нужный размр с конфига/функции
- сделать getNativeResource() для каждого объекта - он возвращает нативный тип типа ID3D11ShaderResourceView - но нужно сделать так чтобы не тянуло d3d11.h
- почистить CreateTexture1D/CreateTexture2D/CreateTexture3D - дублирующийся код
- StateCache