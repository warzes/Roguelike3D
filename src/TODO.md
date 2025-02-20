RHI:
- кеширование стейтов при установке
- при бинде индекс буфера ставить нужный размр с конфига/функции
- сделать getNativeResource() для каждого объекта - он возвращает нативный тип типа ID3D11ShaderResourceView - но нужно сделать так чтобы не тянуло d3d11.h
- Structured Buffer, Indirect Buffer, UAV Buffer
- возможно объединить Texture1DPtr, Texture2DPtr, Texture3DPtr - а то дублируется код. Тем более что оно наследуется от ID3D11Resource