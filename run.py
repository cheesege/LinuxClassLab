from typing import Optional

from fastapi import FastAPI

app = FastAPI() # 建立一個 Fast API application

@app.get("/") # 指定 api 路徑 (get方法)
def read_root():
    return {"Test": "World"}


@app.get("/api/{user_id}") # 指定 api 路徑 (post方法)
def read_user(
    user_id: int,
    
):
    return {"user_id": user_id}