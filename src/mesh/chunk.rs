use bevy::{
    math::Vec3,
    render::{
        mesh::{Indices, Mesh, PrimitiveTopology},
        render_asset::RenderAssetUsages,
    },
};

// Individual blocks mesh data, idk if this is the best way to do this
pub struct MeshInfo {
    positions: Vec<[f32; 3]>,

    indices: Vec<u32>, // which vertices to build which triangle
}

pub enum BlockFace {
    POSX,
    POSY, // TOP
    POSZ,
    NEGX,
    NEGZ,
    NEGY, // BOTTOM
}

fn generate_block_faces(
    faces: &Vec<BlockFace>,
    tile_size: f32,
    block_id: u8,
    start_pos: Vec3,
) -> MeshInfo {
    let mut positions: Vec<[f32; 3]> = Vec::new();
    let mut indices: Vec<u32> = Vec::new();
    let mut index: u32 = 0;
    for face in faces {
        match face {
            BlockFace::POSX => {
                positions.extend(vec![
                    [
                        start_pos.x + tile_size,
                        start_pos.y + tile_size,
                        start_pos.z,
                    ],
                    [start_pos.x + tile_size, start_pos.y, start_pos.z],
                    [
                        start_pos.x + tile_size,
                        start_pos.y,
                        start_pos.z + tile_size,
                    ],
                    [
                        start_pos.x + tile_size,
                        start_pos.y + tile_size,
                        start_pos.z + tile_size,
                    ],
                ]);

                indices.extend(vec![
                    index,
                    index + 3,
                    index + 2,
                    index + 0,
                    index + 2,
                    index + 1,
                ]);
            }
            BlockFace::POSY => {
                positions.extend(vec![
                    [
                        start_pos.x,
                        start_pos.y + tile_size,
                        start_pos.z + tile_size,
                    ],
                    [start_pos.x, start_pos.y + tile_size, start_pos.z],
                    [
                        start_pos.x + tile_size,
                        start_pos.y + tile_size,
                        start_pos.z,
                    ],
                    [
                        start_pos.x + tile_size,
                        start_pos.y + tile_size,
                        start_pos.z + tile_size,
                    ],
                ]);
                indices.extend(vec![
                    index,
                    index + 3,
                    index + 2,
                    index + 0,
                    index + 2,
                    index + 1,
                ]);
            }
            BlockFace::POSZ => {
                positions.extend(vec![
                    [
                        start_pos.x,
                        start_pos.y + tile_size,
                        start_pos.z + tile_size,
                    ],
                    [start_pos.x, start_pos.y, start_pos.z + tile_size],
                    [
                        start_pos.x + tile_size,
                        start_pos.y,
                        start_pos.z + tile_size,
                    ],
                    [
                        start_pos.x + tile_size,
                        start_pos.y + tile_size,
                        start_pos.z + tile_size,
                    ],
                ]);
                indices.extend(vec![
                    index,
                    index + 1,
                    index + 2,
                    index + 3,
                    index + 0,
                    index + 2,
                ]);
            }
            BlockFace::NEGX => {
                positions.extend(vec![
                    [start_pos.x, start_pos.y + tile_size, start_pos.z],
                    [start_pos.x, start_pos.y, start_pos.z],
                    [start_pos.x, start_pos.y, start_pos.z + tile_size],
                    [
                        start_pos.x,
                        start_pos.y + tile_size,
                        start_pos.z + tile_size,
                    ],
                ]);
                indices.extend(vec![
                    index,
                    index + 1,
                    index + 2,
                    index + 3,
                    index + 0,
                    index + 2,
                ]);
            }
            BlockFace::NEGY => {
                positions.extend(vec![
                    [start_pos.x, start_pos.y, start_pos.z + tile_size],
                    [start_pos.x, start_pos.y, start_pos.z],
                    [start_pos.x + tile_size, start_pos.y, start_pos.z],
                    [
                        start_pos.x + tile_size,
                        start_pos.y,
                        start_pos.z + tile_size,
                    ],
                ]);

                indices.extend(vec![
                    index,
                    index + 1,
                    index + 2,
                    index + 3,
                    index + 0,
                    index + 2,
                ]);
            }
            BlockFace::NEGZ => {
                positions.extend(vec![
                    [start_pos.x, start_pos.y + tile_size, start_pos.z],
                    [start_pos.x, start_pos.y, start_pos.z],
                    [start_pos.x + tile_size, start_pos.y, start_pos.z],
                    [
                        start_pos.x + tile_size,
                        start_pos.y + tile_size,
                        start_pos.z,
                    ],
                ]);
                indices.extend(vec![
                    index,
                    index + 3,
                    index + 2,
                    index + 0,
                    index + 2,
                    index + 1,
                ]);
            }
        }

        index += 4;
    }
    MeshInfo { positions, indices }
}

pub fn generate_chunk_mesh() -> Mesh {
    // get block data
    let mut chunk_mesh_data = MeshInfo {
        positions: Vec::new(),
        indices: Vec::new(),
    };
    for y in 0..=2 {
        for z in 0..=2 {
            for x in 0..=2 {
                let tmp = generate_block_faces(
                    &vec![
                        BlockFace::POSX,
                        BlockFace::POSZ,
                        BlockFace::POSY,
                        BlockFace::NEGX,
                        BlockFace::NEGY,
                        BlockFace::NEGZ,
                    ],
                    1.0,
                    0,
                    Vec3 {
                        x: x as f32,
                        y: y as f32,
                        z: z as f32,
                    },
                );
                chunk_mesh_data.indices.extend(
                    tmp.indices
                        .iter()
                        .map(|x| x + chunk_mesh_data.indices.len() as u32)
                        .collect::<Vec<u32>>(),
                );
                chunk_mesh_data.positions.extend(tmp.positions);
            }
        }
    }

    // Keep the mesh data accessible in future frames for mutating
    Mesh::new(
        PrimitiveTopology::TriangleList,
        RenderAssetUsages::MAIN_WORLD | RenderAssetUsages::RENDER_WORLD,
    )
    .with_inserted_attribute(Mesh::ATTRIBUTE_POSITION, chunk_mesh_data.positions)
    .with_inserted_indices(Indices::U32(chunk_mesh_data.indices))
}
