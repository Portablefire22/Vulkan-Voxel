use bevy::{
    log::{debug, info},
    math::{FloatExt, Vec3},
    render::{
        mesh::{Indices, Mesh, PrimitiveTopology},
        render_asset::RenderAssetUsages,
    },
    ui::debug,
};

use rand::Rng;

// Individual blocks mesh data, idk if this is the best way to do this
pub struct MeshInfo {
    positions: Vec<[f32; 3]>,
    colours: Vec<[f32; 4]>,
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
    let mut colours: Vec<[f32; 4]> = Vec::new();
    let mut index: u32 = 0;
    let mut rng = rand::thread_rng();
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
        /*colours.extend(vec![
            [rng.gen::<f32>(), rng.gen::<f32>(), rng.gen::<f32>(), 1.0],
            [rng.gen::<f32>(), rng.gen::<f32>(), rng.gen::<f32>(), 1.0],
            [rng.gen::<f32>(), rng.gen::<f32>(), rng.gen::<f32>(), 1.0],
            [rng.gen::<f32>(), rng.gen::<f32>(), rng.gen::<f32>(), 1.0],
        ]);*/

        colours.extend(vec![
            [
                start_pos.x / 16.0,
                start_pos.y / 16.0,
                start_pos.z / 16.0,
                1.0,
            ],
            [
                start_pos.x / 16.0,
                start_pos.y / 16.0,
                start_pos.z / 16.0,
                1.0,
            ],
            [
                start_pos.x / 16.0,
                start_pos.y / 16.0,
                start_pos.z / 16.0,
                1.0,
            ],
            [
                start_pos.x / 16.0,
                start_pos.y / 16.0,
                start_pos.z / 16.0,
                1.0,
            ],
        ]);

        index += 4;
    }
    MeshInfo {
        positions,
        colours,
        indices,
    }
}

pub fn generate_chunk_mesh() -> Mesh {
    // get block data
    let mut chunk_mesh_data = MeshInfo {
        positions: Vec::new(),
        colours: Vec::new(),
        indices: Vec::new(),
    };
    for y in 0..=16 {
        for z in 0..=16 {
            for x in 0..=16 {
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
                let indices_offset = chunk_mesh_data.positions.len();
                chunk_mesh_data.indices.extend(
                    tmp.indices
                        .iter()
                        .map(|x| x + indices_offset as u32)
                        .collect::<Vec<u32>>(),
                );
                chunk_mesh_data.positions.extend(tmp.positions);
                chunk_mesh_data.colours.extend(tmp.colours);
            }
        }
    }

    // Keep the mesh data accessible in future frames for mutating
    Mesh::new(
        PrimitiveTopology::TriangleList,
        RenderAssetUsages::MAIN_WORLD | RenderAssetUsages::RENDER_WORLD,
    )
    .with_inserted_attribute(Mesh::ATTRIBUTE_COLOR, chunk_mesh_data.colours)
    .with_inserted_attribute(Mesh::ATTRIBUTE_POSITION, chunk_mesh_data.positions)
    .with_inserted_indices(Indices::U32(chunk_mesh_data.indices))
}
